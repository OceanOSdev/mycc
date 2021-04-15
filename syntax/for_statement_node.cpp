#include "for_statement_node.h"
#include "syntax_kind.h"

namespace Syntax {

ForStatementNode::ForStatementNode(SyntaxToken* token, ExpressionNode* ini, ExpressionNode* condition, ExpressionNode* inc, StatementNode* body) :
    StatementNode(token), init_expr(ini), cond_expr(condition), inc_expr(inc), body_stmt(body) {}

ForStatementNode::~ForStatementNode() {
    if (init_expr != nullptr) delete init_expr;
    if (cond_expr != nullptr) delete cond_expr;
    if (inc_expr != nullptr) delete inc_expr;
    delete body_stmt;
}

/*
 * The initial expression that runs at the start of the for loop.
 */
ExpressionNode* ForStatementNode::initial_expression() const { 
    return init_expr; 
}

/*
 * The conditional expression that runs before every loop.
 */
ExpressionNode* ForStatementNode::conditional_expression() const { 
    return cond_expr; 
}

/*
 * The third expression that runs after every loop.
 */
ExpressionNode* ForStatementNode::third_expression() const { 
    return inc_expr; 
}

/*
 * The body statement that runs every loop.
 */
StatementNode* ForStatementNode::body_statement() const { 
    return body_stmt; 
}

/*
 * Whether or not this for loop contains an initial expression.
 */
bool ForStatementNode::has_initial_expression() const { 
    return init_expr != nullptr; 
}

/*
 * Whether or not this for loop contains a conditional expression.
 */
bool ForStatementNode::has_conditional_expression() const { 
    return cond_expr != nullptr; 
}

/*
 * Whether or not this for loop contains a third expression.
 */
bool ForStatementNode::has_third_expression() const { 
    return inc_expr != nullptr; 
}

SyntaxKind ForStatementNode::kind() const {
    return SyntaxKind::ForStatement;
}


}