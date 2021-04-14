#include "function_prototype_node.h"
#include "syntax_kind.h"

namespace Syntax {

FunctionPrototypeNode::FunctionPrototypeNode(FunctionDeclarationNode* func) : 
    m_func_decl(func) {}

FunctionPrototypeNode::~FunctionPrototypeNode() {
    delete m_func_decl;
}

/*
 * The function that this prototype is declaring.
 */
FunctionDeclarationNode* FunctionPrototypeNode::function_declaration() const { 
    return m_func_decl; 
}

SyntaxKind FunctionPrototypeNode::kind() const{
    return SyntaxKind::FunctionPrototype;
}

}