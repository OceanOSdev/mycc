#pragma once
#ifndef BINDER_H
#define BINDER_H

#include <vector>
#include <string>

class Logger;

namespace Syntax {
    class StatementNode;
}

namespace Binding {
// Forward decs
class BoundStatementNode;

class Binder {
private:
    std::vector<std::string> m_diagnostics;
    bool m_err_flag;
    Logger* m_logger;
    BoundStatementNode* BindStatement(Syntax::StatementNode* statement);
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