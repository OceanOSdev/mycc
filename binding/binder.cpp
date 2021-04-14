#include "binder.h"
#include "bound_statement_node.h"
#include "bound_expression_statement_node.h"
#include "../syntax/statement_node.h"
#include "../logger.h"
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


BoundStatementNode* Binder::BindStatement(Syntax::StatementNode* statement) {
    if (statement != nullptr) m_logger->log_info("bleh");
    return nullptr;
}

}