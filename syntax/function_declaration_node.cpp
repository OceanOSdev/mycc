#include "function_declaration_node.h"

namespace Syntax {

FunctionDeclarationNode::FunctionDeclarationNode(std::string type_identifier,
                        std::string function_name,
                        std::vector<FormalParameterNode*> params,
                        bool is_struct,
                        bool is_const) :
    m_type_identifier(type_identifier),
    m_function_name(function_name),
    m_parameters(params),
    m_is_struct(is_struct),
    m_is_const(is_const) {}

FunctionDeclarationNode::~FunctionDeclarationNode() {
    std::vector<FormalParameterNode*>::iterator param_iter;
    for (param_iter = m_parameters.begin(); param_iter != m_parameters.end(); param_iter++)
        delete *param_iter;
}

/*
 * The return type for this function.
 */
std::string FunctionDeclarationNode::type_identifier() const {
    return m_type_identifier; 
}
    
/*
 * The name of this function.
 */
std::string FunctionDeclarationNode::function_name() const {
    return m_function_name; 
}
    
/*
 * The list of parameters for this function.
 */
std::vector<FormalParameterNode*> FunctionDeclarationNode::parameters() const {
    return m_parameters; 
}
    
/*
 * Whether the function's return type is a struct type or not.
 */
bool FunctionDeclarationNode::is_struct() const {
    return m_is_struct; 
}
    
/*
 * Whether the function's return type is a constant type or not.
 */
bool FunctionDeclarationNode::is_const() const {
    return m_is_const; 
}

}