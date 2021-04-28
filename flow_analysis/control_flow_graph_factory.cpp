#include <stdexcept>
#include <algorithm>

#include "control_flow_graph_factory.h"
#include "control_flow_graph.h"
#include "basic_block_factory.h"
#include "basic_block_branch.h"
#include "basic_block.h"

#include "../binding/bound_literal_val_expression_node.h"
#include "../binding/bound_statement_node.h"
#include "../binding/bound_goto_statement_node.h"
#include "../binding/bound_conditional_goto_statement_node.h"
#include "../binding/bound_return_statement_node.h"
#include "../binding/bound_label_statement_node.h"
#include "../binding/bound_unary_expression_node.h"
#include "../binding/bound_node_kind.h"

#include "../syntax/syntax_token.h"

namespace FlowAnalysis {

//one off non class method

/**
 * @brief Negates a bound expression.
 * 
 * Assumes that the expression is a conditional expression.
 * 
 * @param expr The expression to negate.
 * @return The negated expression.
 */
Binding::BoundExpressionNode* negate_expression(Binding::BoundExpressionNode* expr) {
    if (expr->kind() == Binding::BoundNodeKind::LiteralExpression) {
        auto lit = dynamic_cast<Binding::BoundLiteralValExpressionNode*>(expr);
        return new Binding::BoundLiteralValExpressionNode(
            (int)(!Binding::BoundLiteralValExpressionNode::is_nonzero(lit))); // this *shouldn't* bite me in the ass...
            //maybe I should make a bound node factory
    }
    auto bang_op = Binding::BoundUnaryOperatorNode::Bind(Syntax::token_type_t::BANG, expr->type());
    return new Binding::BoundUnaryExpressionNode(bang_op, expr);
}

ControlFlowGraphFactory::ControlFlowGraphFactory() :
    m_start_block(new BasicBlock(true)),
    m_end_block(new BasicBlock(false)) {}

void ControlFlowGraphFactory::connect_blocks(BasicBlock* from, BasicBlock* to, Binding::BoundExpressionNode* condition) {
    Binding::BoundExpressionNode* cond_expr = condition; // don't want to mutate argument.
    if (condition != nullptr && condition->kind() == Binding::BoundNodeKind::LiteralExpression) {
        auto lit_expr = dynamic_cast<Binding::BoundLiteralValExpressionNode*>(condition);
        if (Binding::BoundLiteralValExpressionNode::is_nonzero(lit_expr))
            cond_expr = nullptr; // lit val is true so jump is effectively unconditional.
        else
            return; // lit val is false so jump will never happen, don't connect these blocks.
    }

    auto b_branch = new BasicBlockBranch(from, to, cond_expr);
    from->add_outgoing(b_branch);
    to->add_incoming(b_branch);
    m_branches.push_back(b_branch);
}

ControlFlowGraph* ControlFlowGraphFactory::build(std::vector<BasicBlock*> blocks) {
    if (blocks.empty())
        connect_blocks(m_start_block, m_end_block);
    else
        connect_blocks(m_start_block, blocks.front());
    
    for (auto block : blocks) {
        for (auto statement : block->statements()) {
            m_block_from_statement_map.insert(std::make_pair(statement, block));
            if (statement->kind() == Binding::BoundNodeKind::LabelStatement)
                m_block_from_label_map.insert(
                    std::make_pair((dynamic_cast<Binding::BoundLabelStatementNode*>(statement))->label(), block));
        }
    }

    for (std::vector<BasicBlock*>::size_type i = 0; i < blocks.size(); i ++) {
        BasicBlock* current = blocks[i];
        BasicBlock* next = i == blocks.size() - 1 ? m_end_block : blocks[i + 1];

        for (auto statement : current->statements()) {
            bool is_last_statement_in_block = statement == current->statements().back();
            switch (statement->kind()) {
                case Binding::BoundNodeKind::GotoStatement:
                {
                    auto goto_statement = dynamic_cast<Binding::BoundGotoStatementNode*>(statement);
                    auto destination_block = m_block_from_label_map[goto_statement->label()];
                    connect_blocks(current, destination_block);
                    break;
                }
                case Binding::BoundNodeKind::ConditionalGotoStatement:
                {
                    auto conditional_goto_statement = dynamic_cast<Binding::BoundConditionalGotoStatementNode*>(statement);
                    auto consequence_block = m_block_from_label_map[conditional_goto_statement->label()];
                    auto alternative_block = next;
                    
                    auto negated_condition = negate_expression(conditional_goto_statement->condition());
                    auto consequence_condition = conditional_goto_statement->jump_if_true() 
                        ? conditional_goto_statement->condition() : negated_condition;
                    auto alternative_condition = conditional_goto_statement->jump_if_true()
                        ? negated_condition : conditional_goto_statement->condition();

                    connect_blocks(current, consequence_block, consequence_condition);
                    connect_blocks(current, alternative_block, alternative_condition);
                    break;
                }
                case Binding::BoundNodeKind::ReturnStatement: connect_blocks(current, m_end_block);
                    break;
                case Binding::BoundNodeKind::EmptyStatement: [[fallthrough]];
                case Binding::BoundNodeKind::VariableGroupDeclaration: [[fallthrough]];
                case Binding::BoundNodeKind::VariableDeclaration: [[fallthrough]];
                case Binding::BoundNodeKind::LabelStatement: [[fallthrough]];
                case Binding::BoundNodeKind::ExpressionStatement:
                    if (is_last_statement_in_block)
                        connect_blocks(current, next);
                    break;
                default:
                    throw std::runtime_error("Unexpected statement while building control flow graph");
            }
        }
    }

    bool should_rescan;
    do {
        should_rescan = false;
        for (auto block : blocks) {
            if (block->incoming().empty()) {
                remove_block(blocks, block);
                should_rescan = true;
                break; // break out of the for loop and rescan the block list
                /// TODO: Maybe add a diagnostic warning the user that code is unreachable? 
            }
        }
    } while (should_rescan);

    blocks.insert(blocks.begin(), m_start_block);
    blocks.push_back(m_end_block);
    return new ControlFlowGraph(m_start_block, m_end_block, blocks, m_branches);
}


void ControlFlowGraphFactory::remove_block(std::vector<BasicBlock*>& block_list, BasicBlock* block_to_remove) {
    for (auto branch : block_to_remove->incoming()) {
        branch->from()->remove_outgoing(branch);
        auto iter_end = std::remove(m_branches.begin(), m_branches.end(), branch);
        m_branches.erase(iter_end, m_branches.end());
    }

    for (auto branch: block_to_remove->outgoing()) {
        branch->to()->remove_incoming(branch);
        auto iter_end = std::remove(m_branches.begin(), m_branches.end(), branch);
        m_branches.erase(iter_end, m_branches.end());
    }

    auto iter_end = std::remove(block_list.begin(), block_list.end(), block_to_remove);
    block_list.erase(iter_end, block_list.end());
}

}