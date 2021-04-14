#include <stdexcept>
#include "binder.h"
#include "bound_statement_node.h"
#include "bound_expression_statement_node.h"
#include "bound_error_expression_node.h"
#include "../syntax/statement_node.h"
#include "../logger.h"
#include "../symbols/type_symbol.h"
#include "../syntax/syntax_kind.h"
#include "../syntax/statement_node.h"
#include "../syntax/expression_node.h"
#include "../syntax/expression_statement_node.h"


namespace Binding {

/*
 * List of strings describing any errors that may
 * have occured during binding.
 */
std::vector<std::string> Binder::diagnostics() const {
    return m_diagnostics;
}

/*
 * Bool flag which returns true if there was an error
 * during binding.
 */
bool Binder::err_flag() const {
    return m_err_flag;
}


BoundStatementNode* Binder::bind_statement(Syntax::StatementNode* statement) {
    auto syntaxKind = statement->kind();
    switch (syntaxKind) {
        case Syntax::SyntaxKind::BlockStatement: 
            break;
        case Syntax::SyntaxKind::BreakStatement: 
            break;
        case Syntax::SyntaxKind::ContinueStatement: 
            break;
        case Syntax::SyntaxKind::DoWhileStatement: 
            break;
        case Syntax::SyntaxKind::ExpressionStatement:
            return bind_expression_statement(dynamic_cast<Syntax::ExpressionStatementNode*>(statement)); 
        case Syntax::SyntaxKind::ForStatement: 
            break;
        case Syntax::SyntaxKind::StructDeclaration: 
            break;
        case Syntax::SyntaxKind::VariableDeclaration: 
            break;
        case Syntax::SyntaxKind::IfStatement: 
            break;
        case Syntax::SyntaxKind::ReturnStatement: 
            break;
        case Syntax::SyntaxKind::WhileStatement: 
            break;
        default:
            throw std::runtime_error("Unexpected syntax while binding statement.");
    }

    return nullptr;
}

BoundStatementNode* Binder::bind_expression_statement(Syntax::ExpressionStatementNode* expressionStatement) {
    auto expression = bind_expression(expressionStatement->expression(), true);
    return new BoundExpressionStatementNode(expression);
}

BoundExpressionNode* Binder::bind_expression(Syntax::ExpressionNode* expression, bool canBeVoid) {
    auto result = bind_expression_internal(expression);
    if (!canBeVoid && Symbols::TypeSymbol::are_types_equivalent(result->type(), &Symbols::TypeSymbol::Void)) {
        m_diagnostics.push_back("Expression must have value.");
        return new BoundErrorExpressionNode();
    }
    return result;
}

BoundExpressionNode* Binder::bind_expression_internal(Syntax::ExpressionNode* expression) {
    auto syntaxKind = expression->kind();
    switch (syntaxKind) {
        case Syntax::SyntaxKind::AssignmentExpression:
            break;
        case Syntax::SyntaxKind::BinaryExpression:
            break;
        case Syntax::SyntaxKind::CallExpression:
            break;
        case Syntax::SyntaxKind::CastExpression:
            break;
        case Syntax::SyntaxKind::DecrementExpression:
            break;
        case Syntax::SyntaxKind::IncrementExpression:
            break;
        case Syntax::SyntaxKind::IndexExpression:
            break;
        case Syntax::SyntaxKind::LiteralValExpression:
            break;
        case Syntax::SyntaxKind::MemberExpression:
            break;
        case Syntax::SyntaxKind::NameExpression:
            break;
        case Syntax::SyntaxKind::TernaryExpression:
            break;
        case Syntax::SyntaxKind::UnaryExpression:
            break;
        default:
            throw std::runtime_error("Unexpected syntax while binding expression.");
    }

    return nullptr;
}


}