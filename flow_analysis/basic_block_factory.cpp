#include <stdexcept>
#include "basic_block_factory.h"
#include "basic_block.h"
#include "../binding/bound_block_statement_node.h"
#include "../binding/bound_node_kind.h"

namespace FlowAnalysis {

/**
 * @brief Indicates the builder should start a new block.
 * 
 * @note This method exists only to alleviate cognative.
 *       It literally just calls end_block. 
 */
void BasicBlockFactory::start_block() {
    end_block();
}

/**
 * @brief Ends the current block that in the builder. 
 */
void BasicBlockFactory::end_block() {
    if (m_statements.size() > 0) {
        auto b_block = new BasicBlock();
        b_block->add_all_statements(m_statements);
        m_blocks.push_back(b_block);
        m_statements.clear();
    }
}

BasicBlockFactory::BasicBlockFactory() { }

std::vector<BasicBlock*> BasicBlockFactory::build(Binding::BoundBlockStatementNode* block) {
    for (auto statement : block->statements()) {
        switch (statement->kind()) {
            case Binding::BoundNodeKind::LabelStatement:
                start_block(); // since we don't add labels to anything other than loops yet.
                m_statements.push_back(statement);
                break;
            case Binding::BoundNodeKind::GotoStatement: [[fallthrough]];
            case Binding::BoundNodeKind::ConditionalGotoStatement: [[fallthrough]];
            case Binding::BoundNodeKind::ReturnStatement:
                m_statements.push_back(statement);
                end_block(); // gotos and returns should be at the end of the block
                break;
            case Binding::BoundNodeKind::EmptyStatement: [[fallthrough]];
            case Binding::BoundNodeKind::VariableGroupDeclaration: [[fallthrough]];
            case Binding::BoundNodeKind::VariableDeclaration: [[fallthrough]];
            case Binding::BoundNodeKind::ExpressionStatement:
                m_statements.push_back(statement);
                break;
            default:
                throw std::runtime_error("Unexpected statement while analyzing control flow.\n\tStatement type: " + std::to_string(statement->kind()));
        }
    }

    end_block(); //if we haven't already.
    return std::vector<BasicBlock*>(m_blocks);
}


}