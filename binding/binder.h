#pragma once
#ifndef BINDER_H
#define BINDER_H

#include <vector>
#include <string>

class Logger;

namespace Syntax {
    class StatementNode;
    class ExpressionStatementNode;
    class ExpressionNode;
}

namespace Binding {
// Forward decs
class BoundStatementNode;
class BoundExpressionNode;

class Binder {
private:
    std::vector<std::string> m_diagnostics;
    bool m_err_flag;
    Logger* m_logger;

    /* statement bindings */

    BoundStatementNode* bind_statement(Syntax::StatementNode* statement);
    BoundStatementNode* bind_expression_statement(Syntax::ExpressionStatementNode* expressionStatement);

    /* expression bindings */

    BoundExpressionNode* bind_expression(Syntax::ExpressionNode* expression, bool canBeVoid = false);
    BoundExpressionNode* bind_expression_internal(Syntax::ExpressionNode* expression);
public:

    /*
     * List of strings describing any errors that may
     * have occured during binding.
     */
    std::vector<std::string> diagnostics() const;

    /*
     * Bool flag which returns true if there was an error
     * during binding.
     */
    bool err_flag() const;
};

}

#endif