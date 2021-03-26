#ifndef SYNTAX_FUNCTION_PROTOTYPE_NODE_H
#define SYNTAX_FUNCTION_PROTOTYPE_NODE_H

#include "global_declaration_node.h"
#include "function_declaration_node.h"

namespace Syntax {

class FunctionPrototypeNode : public GlobalDeclarationNode {
private:
    FunctionDeclarationNode* m_func_decl;
public:
    FunctionPrototypeNode(FunctionDeclarationNode* func) : m_func_decl(func) {}
    
    ~FunctionPrototypeNode() {
        delete m_func_decl;
    }

    /*
     * The function that this prototype is declaring.
     */
    FunctionDeclarationNode* function_declaration() const { return m_func_decl; }
};

}

#endif