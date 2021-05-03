#include "tree_rewriter.h"

#include "../flow_analysis/control_flow_graph.h"
#include "../flow_analysis/basic_block.h"

#include "bound_statement_node.h"
#include "bound_expression_statement_node.h"
#include "bound_error_expression_node.h"
#include "bound_block_statement_node.h"
#include "bound_return_statement_node.h"
#include "bound_empty_statement_node.h"
#include "bound_label_statement_node.h"
#include "bound_if_statement_node.h"
#include "bound_goto_statement_node.h"
#include "bound_for_statement_node.h"
#include "bound_do_while_statement_node.h"
#include "bound_while_statement_node.h"
#include "bound_label_statement_node.h"
#include "bound_goto_statement_node.h"
#include "bound_conditional_goto_statement_node.h"
#include "bound_variable_group_declaration_node.h"
#include "bound_literal_val_expression_node.h"
#include "bound_index_expression_node.h"
#include "bound_struct_declaration_node.h"
#include "bound_variable_reference_expression_node.h"
#include "bound_member_access_expression_node.h"
#include "bound_cast_expression_node.h"
#include "bound_binary_expression_node.h"
#include "bound_unary_expression_node.h"
#include "bound_assignment_expression_node.h"
#include "bound_call_expression_node.h"
#include "bound_ternary_expression_node.h"
#include "bound_increment_expression_node.h"
#include "bound_global_declaration_node.h"
#include "bound_global_statement_node.h"
#include "bound_function_definition_node.h"

#include "../symbols/type_symbol.h"

#include <string>
#include <stdexcept>
#include <stack>
#include <set>


namespace Binding {

TreeRewriter::TreeRewriter() : m_label_count(0) {}
TreeRewriter::TreeRewriter(int label_offset) : m_label_count(label_offset) {}

BoundLabel* TreeRewriter::generate_label() {
    ++m_label_count;
    std::string label = "L" + std::to_string(m_label_count);
    return new BoundLabel(label);
}

BoundGlobalDeclarationNode* TreeRewriter::rewrite_global_statement(BoundGlobalStatementNode* bound_global_statement) {
    const int default_label_counter = 0;
    auto statement = rewrite(bound_global_statement->statement(), default_label_counter, true);

    // There should only be one statement in the returned flattened bound block statement
    // so we grab that, wrap it in a BoundGlobalStatementNode, and return it.
    return new BoundGlobalStatementNode(statement->statements().at(0));
}


std::vector<BoundGlobalDeclarationNode*> TreeRewriter::rewrite(std::vector<BoundGlobalDeclarationNode*> bound_program) {
    int label_counter = 0;
    std::vector<BoundGlobalDeclarationNode*> lowered_program;
    for (auto dec : bound_program) {
        auto kind = dec->kind();
        switch (kind) {
            case Binding::BoundNodeKind::GlobalStatement:
            {
                auto gdec = dynamic_cast<BoundGlobalStatementNode*>(dec);
                lowered_program.push_back(rewrite_global_statement(gdec));
                break;
            }
            case BoundNodeKind::FunctionDefinition:
            {
                auto func = dynamic_cast<BoundFunctionDefinitionNode*>(dec);
                BoundBlockStatementNode* rewritten = nullptr;
                label_counter = TreeRewriter::rewrite(func->statements(), rewritten, label_counter);
                auto bound_function_def = new BoundFunctionDefinitionNode(func->function_symbol(), rewritten);
                lowered_program.push_back(bound_function_def);
                break;
            }
            default:
                throw std::runtime_error("Invalid global declaration kind.");
        }
    }

    return lowered_program;
}

/*
 * Does the same thing as the other rewrite function, but
 * returns the new BoundBlockStatementNode through an "out" parameter
 * and returns the label count after rewriting through the usual means.
 */
int TreeRewriter::rewrite(BoundStatementNode* in_statement, BoundBlockStatementNode*& out_statement, int label_offset, bool is_global_statement) {
    // cant just call the other rewrite method since I need access to an instance
    // of the rewriter class to grab the label count.
    auto rewriter = new TreeRewriter(label_offset);
    auto lowered_statement = flatten(rewriter->rewrite_statement(in_statement));
    out_statement = is_global_statement ? lowered_statement : remove_dead_code(lowered_statement);
    return rewriter->m_label_count;
}

BoundBlockStatementNode* TreeRewriter::rewrite(BoundStatementNode* statement, int label_offset, bool is_global_statement) {
    auto rewriter = new TreeRewriter(label_offset);
    auto lowered_statement = flatten(rewriter->rewrite_statement(statement));
    return is_global_statement ? lowered_statement : remove_dead_code(lowered_statement); // global statements aren't really "branching" code ya know? So no need to remove dead code.
}

BoundBlockStatementNode* TreeRewriter::flatten(BoundStatementNode* statement) {
    std::vector<BoundStatementNode*> flattened_statement_list;
    std::stack<BoundStatementNode*> remaining_statements;
    remaining_statements.push(statement);

    while (!remaining_statements.empty()) {
        auto current = remaining_statements.top();
        remaining_statements.pop();

        if (auto block_statement = dynamic_cast<BoundBlockStatementNode*>(current)) {
            auto statements = block_statement->statements();
            for (auto r_iter = statements.rbegin(); r_iter != statements.rend(); r_iter++) {
                remaining_statements.push(*r_iter);
            }
        } else {
            flattened_statement_list.push_back(current);
        }
    }

    return new BoundBlockStatementNode(flattened_statement_list);
}

BoundBlockStatementNode* TreeRewriter::remove_dead_code(BoundBlockStatementNode* statement) {
    auto control_flow_graph = FlowAnalysis::ControlFlowGraph::create(statement);
    std::set<BoundStatementNode*> reachable_statements;
    for (auto blocks : control_flow_graph->block_list()) {
        for (auto stmt : blocks->statements()) {
            reachable_statements.insert(stmt);
        }
    }

    auto new_statements = std::vector<BoundStatementNode*> (statement->statements());
    for (long i = new_statements.size() - 1; i >= 0; i--) {
        if (!reachable_statements.contains(new_statements[i])) {
            new_statements.erase(new_statements.begin() + i);
        }
    }

    return new BoundBlockStatementNode(new_statements);
}


BoundStatementNode* TreeRewriter::rewrite_statement(BoundStatementNode* statement) {
    auto kind = statement->kind();
    switch (kind) {
        case BoundNodeKind::ExpressionStatement:
            return rewrite_expression_statement(dynamic_cast<BoundExpressionStatementNode*>(statement));
        case BoundNodeKind::BlockStatement:
            return rewrite_block_statement(dynamic_cast<BoundBlockStatementNode*>(statement));
        case BoundNodeKind::VariableGroupDeclaration:
            return rewrite_variable_group_declaration(dynamic_cast<BoundVariableGroupDeclarationNode*>(statement));
        case BoundNodeKind::VariableDeclaration:
            return rewrite_variable_declaration(dynamic_cast<BoundVariableDeclarationNode*>(statement));
        case BoundNodeKind::IfStatement:
            return rewrite_if_statement(dynamic_cast<BoundIfStatementNode*>(statement));
        case BoundNodeKind::ForStatement:
            return rewrite_for_statement(dynamic_cast<BoundForStatementNode*>(statement));
        case BoundNodeKind::WhileStatement:
            return rewrite_while_statement(dynamic_cast<BoundWhileStatementNode*>(statement));
        case BoundNodeKind::DoWhileStatement:
            return rewrite_do_while_statement(dynamic_cast<BoundDoWhileStatementNode*>(statement));
        case BoundNodeKind::ReturnStatement:
            return rewrite_return_statement(dynamic_cast<BoundReturnStatementNode*>(statement));
        case BoundNodeKind::StructDeclaration:
            return rewrite_struct_declaration(dynamic_cast<BoundStructDeclarationNode*>(statement));
        case BoundNodeKind::LabelStatement:
            return rewrite_label_statement(dynamic_cast<BoundLabelStatementNode*>(statement));
        case BoundNodeKind::GotoStatement:
            return rewrite_goto_statement(dynamic_cast<BoundGotoStatementNode*>(statement));
        case BoundNodeKind::ConditionalGotoStatement:
            return rewrite_conditional_goto_statement(dynamic_cast<BoundConditionalGotoStatementNode*>(statement));
        default:
            throw std::runtime_error("Unexpected bound node while rewriting statement.");
    }
}

BoundStatementNode* TreeRewriter::rewrite_expression_statement(BoundExpressionStatementNode* expression_statement) {
    auto expression = rewrite_expression(expression_statement->expression());
    if (expression == expression_statement->expression())
        return expression_statement;
    
    return new BoundExpressionStatementNode(expression);
}

BoundStatementNode* TreeRewriter::rewrite_block_statement(BoundBlockStatementNode* block_statement) {
    std::vector<BoundStatementNode*> statements;
    bool requires_rewrite = false;
    auto decl_list_size = block_statement->statements().size();
    for (std::vector<BoundStatementNode*>::size_type i = 0; i < decl_list_size; i++) {
        auto old_statement = block_statement->statements()[i];
        auto new_statement = rewrite_statement(old_statement);
        if (old_statement != new_statement)
            requires_rewrite = true;
        
        statements.push_back(new_statement);
    }

    if (!requires_rewrite) return block_statement;
    return new BoundBlockStatementNode(statements);
}

BoundStatementNode* TreeRewriter::rewrite_variable_group_declaration(BoundVariableGroupDeclarationNode* var_group_decl) {
    std::vector<BoundVariableDeclarationNode*> var_decls;
    bool requires_rewrite = false;
    auto decl_list_size = var_group_decl->variable_declarations().size();
    for (std::vector<BoundVariableDeclarationNode*>::size_type i = 0; i < decl_list_size; i++) {
        auto old_var_decl = var_group_decl->variable_declarations()[i];
        auto new_var_decl = rewrite_statement(old_var_decl);
        if (old_var_decl != new_var_decl)
            requires_rewrite = true;
        
        var_decls.push_back(dynamic_cast<BoundVariableDeclarationNode*>(new_var_decl));
    }

    if (!requires_rewrite) return var_group_decl;
    return new BoundVariableGroupDeclarationNode(var_decls);
}

BoundStatementNode* TreeRewriter::rewrite_variable_declaration(BoundVariableDeclarationNode* var_decl) {
    if (!var_decl->is_initialized())
        return var_decl;
    
    auto initializer = rewrite_expression(var_decl->initializer());

    if (initializer == var_decl->initializer())
        return var_decl;
    
    return new BoundVariableDeclarationNode(var_decl->variable_symbol(), initializer); 
}

BoundStatementNode* TreeRewriter::rewrite_if_statement(BoundIfStatementNode* if_statement) {
    if (!if_statement->has_else_statement()) {
        // want to rewrite expresssion
        //
        // if <cond>
        //    <then>
        // -----------> rewrite as
        //
        // goto end if <cond> false
        // <then>
        // end:
        //
        auto end_label = generate_label();
        std::vector<BoundStatementNode*> statements = {
            new BoundConditionalGotoStatementNode(end_label, if_statement->condition(), false),
            if_statement->then_statement(),
            new BoundLabelStatementNode(end_label)
        };

        return rewrite_statement(new BoundBlockStatementNode(statements));
    } else {
        // want to rewrite expresssion
        //
        // if <cond>
        //    <then>
        // else
        //    <else>
        // -----------> rewrite as
        //
        // goto else if <cond> false
        // <then>
        // goto end
        // else:
        // <else>
        // end:
        //
        auto else_label = generate_label();
        auto end_label = generate_label();
        std::vector<BoundStatementNode*> statements = {
            new BoundConditionalGotoStatementNode(else_label, if_statement->condition(), false),
            if_statement->then_statement(),
            new BoundGotoStatementNode(end_label),
            new BoundLabelStatementNode(else_label),
            if_statement->else_statement(),
            new BoundLabelStatementNode(end_label)
        };

        return rewrite_statement(new BoundBlockStatementNode(statements));
    }
}

BoundStatementNode* TreeRewriter::rewrite_for_statement(BoundForStatementNode* for_statement) {
    // want to rewrite expresssion
    //
    // for (<init>; <cond>; <inc>)
    //    <body>
    // -----------> rewrite as
    //
    // <init>
    // continue:
    // goto break if <cond> false
    // <body>
    // <inc>
    // goto continue
    // break:
    //
    auto break_label = for_statement->break_label();
    auto continue_label = for_statement->continue_label();
    std::vector<BoundStatementNode*> statements;
    if (for_statement->has_initial_expression())
        statements.push_back(new BoundExpressionStatementNode(for_statement->initial_expression()));
    statements.push_back(new BoundLabelStatementNode(continue_label));
    if (for_statement->has_condition_expression())
        statements.push_back(new BoundConditionalGotoStatementNode(break_label, for_statement->condition_expression(), false));
    statements.push_back(for_statement->body_statement());
    if (for_statement->has_third_expression())
        statements.push_back(new BoundExpressionStatementNode(for_statement->third_expression()));
    statements.push_back(new BoundGotoStatementNode(continue_label));
    statements.push_back(new BoundLabelStatementNode(break_label));
    return rewrite_statement(new BoundBlockStatementNode(statements));
}

BoundStatementNode* TreeRewriter::rewrite_while_statement(BoundWhileStatementNode* while_statement) {
    // want to rewrite expresssion
    //
    // while <cond>
    //    <body>
    // -----------> rewrite as
    //
    // continue:
    // goto break if <cond> false
    // <body>
    // goto continue
    // break:
    //
    auto break_label = while_statement->break_label();
    auto continue_label = while_statement->continue_label();
    std::vector<BoundStatementNode*> statements = {
        new BoundLabelStatementNode(continue_label),
        new BoundConditionalGotoStatementNode(break_label, while_statement->condition_expression(), false),
        while_statement->body_statement(),
        new BoundGotoStatementNode(continue_label),
        new BoundLabelStatementNode(break_label)
    };
    return rewrite_statement(new BoundBlockStatementNode(statements));
}

BoundStatementNode* TreeRewriter::rewrite_do_while_statement(BoundDoWhileStatementNode* do_while_statement) {
    // want to rewrite expresssion
    //
    // do
    //    <body>
    // while <cond>
    // -----------> rewrite as
    //
    // continue:
    // <body>
    // goto continue if <cond>
    // break:
    //
    auto break_label = do_while_statement->break_label();
    auto continue_label = do_while_statement->continue_label();
    std::vector<BoundStatementNode*> statements = {
        new BoundLabelStatementNode(continue_label),
        do_while_statement->body_statement(),
        new BoundConditionalGotoStatementNode(continue_label, do_while_statement->condition_expression()),
        new BoundLabelStatementNode(break_label)
    };
    return rewrite_statement(new BoundBlockStatementNode(statements));
}

BoundStatementNode* TreeRewriter::rewrite_return_statement(BoundReturnStatementNode* return_statement) {
    BoundExpressionNode* expression = return_statement->expression() == nullptr ? nullptr : rewrite_expression(return_statement->expression());

    if (expression == return_statement->expression())
        return return_statement;
    
    return new BoundReturnStatementNode(expression);
}

BoundStatementNode* TreeRewriter::rewrite_struct_declaration(BoundStructDeclarationNode* struct_declaration) {
    return struct_declaration;
}

BoundStatementNode* TreeRewriter::rewrite_label_statement(BoundLabelStatementNode* label_statement) {
    return label_statement;
}

BoundStatementNode* TreeRewriter::rewrite_goto_statement(BoundGotoStatementNode* goto_statement) {
    return goto_statement;
}

BoundStatementNode* TreeRewriter::rewrite_conditional_goto_statement(BoundConditionalGotoStatementNode* conditional_goto_statement) {
    auto expression = rewrite_expression(conditional_goto_statement->condition());

    if (expression == conditional_goto_statement->condition())
        return conditional_goto_statement;

    return new BoundConditionalGotoStatementNode(conditional_goto_statement->label(), expression, conditional_goto_statement->jump_if_true());
}


BoundExpressionNode* TreeRewriter::rewrite_expression(BoundExpressionNode* expression) {
    auto kind = expression->kind();
    switch (kind) {
        case BoundNodeKind::AssignmentExpression:
            return rewrite_assignment_expression(dynamic_cast<BoundAssignmentExpressionNode*>(expression));
        case BoundNodeKind::BinaryExpression:
            return rewrite_binary_expression(dynamic_cast<BoundBinaryExpressionNode*>(expression));
        case BoundNodeKind::CallExpression:
            return rewrite_call_expression(dynamic_cast<BoundCallExpressionNode*>(expression));
        case BoundNodeKind::CastExpression:
            return rewrite_cast_expression(dynamic_cast<BoundCastExpressionNode*>(expression));
        case BoundNodeKind::DecrementExpression:
            return rewrite_decrement_expression(dynamic_cast<BoundDecrementExpressionNode*>(expression));
        case BoundNodeKind::IncrementExpression:
            return rewrite_increment_expression(dynamic_cast<BoundIncrementExpressionNode*>(expression));
        case BoundNodeKind::IndexExpression:
            return rewrite_index_expression(dynamic_cast<BoundIndexExpressionNode*>(expression));
        case BoundNodeKind::LiteralExpression:
            return rewrite_literal_val_expression(dynamic_cast<BoundLiteralValExpressionNode*>(expression));
        case BoundNodeKind::MemberAccessExpression:
            return rewrite_member_expression(dynamic_cast<BoundMemberAccessExpressionNode*>(expression));
        case BoundNodeKind::VariableReferenceExpression:
            return rewrite_var_reference_expression(dynamic_cast<BoundVariableReferenceExpressionNode*>(expression));
        case BoundNodeKind::TernaryExpression:
            return rewrite_ternary_expression(dynamic_cast<BoundTernaryExpressionNode*>(expression));
        case BoundNodeKind::UnaryExpression:
            return rewrite_unary_expression(dynamic_cast<BoundUnaryExpressionNode*>(expression));
        default:
            throw std::runtime_error("Unexpected bound node while rewriting expression.");
    }
}

BoundExpressionNode* TreeRewriter::rewrite_assignment_expression(BoundAssignmentExpressionNode* assignment_expression) {
    auto expression = rewrite_expression(assignment_expression->expression());
    auto variable = rewrite_expression(assignment_expression->variable_reference());
    auto assign_op_kind = assignment_expression->assignment_op_kind();
    if (assign_op_kind != BoundAssignmentOpKind::ASSIGN) {
        // rewrite 'var <op>= expr' to 'var = (var <op> expr)'
        BoundBinaryOpKind new_op_kind;
        switch(assign_op_kind) {
            case Binding::BoundAssignmentOpKind::PLUS_ASSIGN: new_op_kind = BoundBinaryOpKind::Addition; break;
            case Binding::BoundAssignmentOpKind::MINUS_ASSIGN: new_op_kind = BoundBinaryOpKind::Subtraction; break;
            case Binding::BoundAssignmentOpKind::STAR_ASSIGN: new_op_kind = BoundBinaryOpKind::Multiplication; break;
            case Binding::BoundAssignmentOpKind::SLASH_ASSIGN: new_op_kind = BoundBinaryOpKind::Division; break;
            default: throw std::runtime_error("Invalid state when rewriting assignment expression.");
        }
        auto bin_op = new BoundBinaryOperatorNode(new_op_kind, variable->type(), expression->type(), variable->type());
        auto new_rval = rewrite_expression(new BoundBinaryExpressionNode(bin_op, variable, expression));
        return new BoundAssignmentExpressionNode(BoundAssignmentOpKind::ASSIGN, dynamic_cast<BoundVariableReferenceExpressionNode*>(variable), new_rval);
    }

    if (Symbols::TypeSymbol::requires_bytecode_cast(expression->type(), variable->type()))
        expression = new BoundCastExpressionNode(variable->type(), expression);

    if (expression == assignment_expression->expression() && variable == assignment_expression->variable_reference())
        return assignment_expression;
    
    
    return new BoundAssignmentExpressionNode(assignment_expression->assignment_op_kind(), dynamic_cast<BoundVariableReferenceExpressionNode*>(variable), expression);
}

BoundExpressionNode* TreeRewriter::rewrite_binary_expression(BoundBinaryExpressionNode* binary_expression) {
    auto left = rewrite_expression(binary_expression->left());
    auto right = rewrite_expression(binary_expression->right());

    auto bin_op = binary_expression->op()->op_kind();
    switch (bin_op) {
        case BoundBinaryOpKind::Equals: [[fallthrough]];
        case BoundBinaryOpKind::NotEquals: [[fallthrough]];
        case BoundBinaryOpKind::LessThan: [[fallthrough]];
        case BoundBinaryOpKind::LessThanOrEquals: [[fallthrough]];
        case BoundBinaryOpKind::GreaterThan: [[fallthrough]];
        case BoundBinaryOpKind::GreaterThanOrEquals: [[fallthrough]];
        case BoundBinaryOpKind::LogicalAnd: [[fallthrough]];
        case BoundBinaryOpKind::LogicalOr: return rewrite_binary_conditional_expression(binary_expression);
        default: break;
    }

    // When emitting byte code, we will have to explicitly cast integer types to non integer types
    if (Symbols::TypeSymbol::requires_bytecode_cast(left->type(), binary_expression->op()->type()))
        left = new BoundCastExpressionNode(binary_expression->op()->type(), left);
    else if (Symbols::TypeSymbol::requires_bytecode_cast(right->type(), binary_expression->op()->type()))
        right = new BoundCastExpressionNode(binary_expression->op()->type(), right);
    
    if (left == binary_expression->left() && right == binary_expression->right())
        return binary_expression;
    
    return new BoundBinaryExpressionNode(binary_expression->op(), left, right);
}

BoundExpressionNode* TreeRewriter::rewrite_binary_conditional_expression(BoundBinaryExpressionNode* binary_expression) {
    auto left = rewrite_expression(binary_expression->left());
    auto right = rewrite_expression(binary_expression->right());

    bool is_left_literal = left->kind() == BoundNodeKind::LiteralExpression;
    bool is_right_literal = right->kind() == BoundNodeKind::LiteralExpression;

    const auto true_literal = new BoundLiteralValExpressionNode(1);
    const auto false_literal = new BoundLiteralValExpressionNode(0);

    BoundLiteralValExpressionNode* lit_left = is_left_literal ? dynamic_cast<BoundLiteralValExpressionNode*>(left) : nullptr;
    BoundLiteralValExpressionNode* lit_right = is_right_literal ? dynamic_cast<BoundLiteralValExpressionNode*>(right) : nullptr;

    /*
    if (binary_expression->op()->op_kind() == BoundBinaryOpKind::LogicalOr) {
        if (is_right_literal && !BoundLiteralValExpressionNode::is_nonzero(lit_right))
            return left;
        if (is_left_literal && !BoundLiteralValExpressionNode::is_nonzero(lit_left)) 
            return right;
        return right;
    } else if (binary_expression->op()->op_kind() == BoundBinaryOpKind::LogicalAnd) {
        if (!BoundLiteralValExpressionNode::is_nonzero(lit_left)) 
            return false_literal; // false && <expr> evaluates to false
        return right;
    }
    */


    if (is_left_literal) {
        if (binary_expression->op()->op_kind() == BoundBinaryOpKind::LogicalOr) {
            if (BoundLiteralValExpressionNode::is_nonzero(lit_left)) 
                return true_literal; // true || <expr> evaluates to true
            if (is_right_literal) {
                return BoundLiteralValExpressionNode::is_nonzero(lit_right) ? true_literal : false_literal;
            }
        } else if (binary_expression->op()->op_kind() == BoundBinaryOpKind::LogicalAnd) {
            if (!BoundLiteralValExpressionNode::is_nonzero(lit_left)) 
                return false_literal; // false && <expr> evaluates to false
            if (is_right_literal) {
                return BoundLiteralValExpressionNode::is_nonzero(lit_right) ? true_literal : false_literal;
            }
        }
    }

    // When emitting byte code, we will have to explicitly cast integer types to non integer types
    if (Symbols::TypeSymbol::requires_bytecode_cast(left->type(), binary_expression->op()->type()))
        left = new BoundCastExpressionNode(binary_expression->op()->type(), left);
    else if (Symbols::TypeSymbol::requires_bytecode_cast(right->type(), binary_expression->op()->type()))
        right = new BoundCastExpressionNode(binary_expression->op()->type(), right);
    
    if (left == binary_expression->left() && right == binary_expression->right())
        return binary_expression;
    
    return new BoundBinaryExpressionNode(binary_expression->op(), left, right);
}

BoundExpressionNode* TreeRewriter::rewrite_call_expression(BoundCallExpressionNode* call_expression) {
    bool requires_rewrite = false;
    std::vector<BoundExpressionNode*> new_args;
    auto num_args = call_expression->arguments().size();
    for (std::vector<BoundExpressionNode*>::size_type i = 0; i < num_args; i++) {
        auto old_arg = call_expression->arguments()[i];
        auto new_arg = rewrite_expression(old_arg);
        if (old_arg != new_arg)
            requires_rewrite = true;
        
        new_args.push_back(new_arg);
    }

    if (!requires_rewrite) return call_expression;
    return new BoundCallExpressionNode(call_expression->function(), new_args);
}

BoundExpressionNode* TreeRewriter::rewrite_cast_expression(BoundCastExpressionNode* cast_expression) {
    auto expression = rewrite_expression(cast_expression->expression());

    if (expression == cast_expression->expression())
        return cast_expression;

    return new BoundCastExpressionNode(cast_expression->type(), expression);
}

BoundExpressionNode* TreeRewriter::rewrite_decrement_expression(BoundDecrementExpressionNode* decrement_expression) {
    auto expression = rewrite_expression(decrement_expression->expression());

    if (expression == decrement_expression->expression())
        return decrement_expression;

    return new BoundDecrementExpressionNode(decrement_expression->notation(), decrement_expression->expression());
}

BoundExpressionNode* TreeRewriter::rewrite_increment_expression(BoundIncrementExpressionNode* increment_expression) {
    auto expression = rewrite_expression(increment_expression->expression());

    if (expression == increment_expression->expression())
        return increment_expression;

    return new BoundIncrementExpressionNode(increment_expression->notation(), increment_expression->expression());
}

BoundExpressionNode* TreeRewriter::rewrite_index_expression(BoundIndexExpressionNode* index_expression) {
    auto expression = rewrite_expression(index_expression->expression());

    if (expression == index_expression->expression())
        return index_expression;
    
    return new BoundIndexExpressionNode(index_expression->variable_reference(), expression);
}

BoundExpressionNode* TreeRewriter::rewrite_literal_val_expression(BoundLiteralValExpressionNode* literal_val_expression) {
    return literal_val_expression;
}

BoundExpressionNode* TreeRewriter::rewrite_member_expression(BoundMemberAccessExpressionNode* member_expression) {
    auto encapsulated_ref = rewrite_expression(member_expression->encapsulated_variable_reference());
    auto encapsulating_ref = rewrite_expression(member_expression->encapsulating_variable_reference());

    if (encapsulated_ref == member_expression->encapsulated_variable_reference() &&
        encapsulating_ref == member_expression->encapsulating_variable_reference())
        return member_expression;
    
    return new BoundMemberAccessExpressionNode(dynamic_cast<BoundMemberAccessExpressionNode*>(encapsulated_ref), dynamic_cast<BoundMemberAccessExpressionNode*>(encapsulating_ref));
}

BoundExpressionNode* TreeRewriter::rewrite_var_reference_expression(BoundVariableReferenceExpressionNode* var_reference_expression) {
    return var_reference_expression;
}

BoundExpressionNode* TreeRewriter::rewrite_ternary_expression(BoundTernaryExpressionNode* ternary_expression) {
    // TODO: might have to rewrite as some weird intermediary thing.
    // but to get stuff off the ground I'll just only rewrite the inner expressions.
    auto conditional = rewrite_expression(ternary_expression->conditional());
    auto true_expr = rewrite_expression(ternary_expression->true_expr());
    auto false_expr = rewrite_expression(ternary_expression->false_expr());

    if (conditional == ternary_expression->conditional() &&
        true_expr == ternary_expression->true_expr() &&
        false_expr == ternary_expression->false_expr())
        return ternary_expression;

    return new BoundTernaryExpressionNode(ternary_expression->type(), conditional, true_expr, false_expr);
}

BoundExpressionNode* TreeRewriter::rewrite_unary_expression(BoundUnaryExpressionNode* unary_expression) {
    auto operand = rewrite_expression(unary_expression->expression());

    bool is_operand_literal = operand->kind() == BoundNodeKind::LiteralExpression;

    const auto true_literal = new BoundLiteralValExpressionNode(1);
    const auto false_literal = new BoundLiteralValExpressionNode(0);

    BoundLiteralValExpressionNode* lit_operand = is_operand_literal ? dynamic_cast<BoundLiteralValExpressionNode*>(operand) : nullptr;
    
    if (is_operand_literal) {
        if (unary_expression->op()->op_kind() == BoundUnaryOpKind::LogicalNegation)
            return BoundLiteralValExpressionNode::is_nonzero(lit_operand) ? false_literal : true_literal;
    }

    if (operand == unary_expression->expression())
        return unary_expression;

    return new BoundUnaryExpressionNode(unary_expression->op(), operand);
}

}