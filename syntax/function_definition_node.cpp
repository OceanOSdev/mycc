#include "function_definition_node.h"
#include "syntax_kind.h"

namespace Syntax {

FunctionDefinitionNode::FunctionDefinitionNode(FunctionDeclarationNode* func,
                       std::vector<Syntax::LocalDeclarationNode*> local_decs,
                       std::vector<Syntax::StatementNode*> statements) : 
    m_func_decl(func),
    m_local_declarations(local_decs),
    m_statements(statements) {}

FunctionDefinitionNode::~FunctionDefinitionNode() {
    delete m_func_decl;
    std::vector<Syntax::LocalDeclarationNode*>::iterator dec_iter;
    for (dec_iter = m_local_declarations.begin(); dec_iter != m_local_declarations.end(); dec_iter++)
        delete *dec_iter;
    std::vector<Syntax::StatementNode*>::iterator stmt_iter;
    for (stmt_iter = m_statements.begin(); stmt_iter != m_statements.end(); stmt_iter++)
        delete *stmt_iter;
}

/*
 * The function that this definition is declaring.
 */
FunctionDeclarationNode* FunctionDefinitionNode::function_declaration() const { 
    return m_func_decl; 
}
    
/*
 * The declarations in the function.
 */
std::vector<Syntax::LocalDeclarationNode*> FunctionDefinitionNode::local_declarations() const { 
    return m_local_declarations; 
}
    
/*
 * The statements in this function after the declarations.
 */
std::vector<Syntax::StatementNode*> FunctionDefinitionNode::statements() const { 
    return m_statements; 
}

SyntaxKind FunctionDefinitionNode::kind() const {
    return SyntaxKind::FunctionDefinition;
}

}