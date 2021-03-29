#ifndef SYNTAX_FUNCTION_DECLARATION_NODE_H
#define SYNTAX_FUNCTION_DECLARATION_NODE_H

#include <string>
#include <vector>
#include "syntax_node.h"
#include "formal_parameter_node.h"

namespace Syntax {

class FunctionDeclarationNode : public SyntaxNode {
private:
    std::string m_type_identifier;
    std::string m_function_name;
    std::vector<FormalParameterNode*> m_parameters;
    bool m_is_struct;
    bool m_is_const;
public:
    FunctionDeclarationNode(std::string type_identifier,
                            std::string function_name,
                            std::vector<FormalParameterNode*> params,
                            bool is_struct = false,
                            bool is_const = false);
    
    ~FunctionDeclarationNode();

    /*
     * The return type for this function.
     */
    std::string type_identifier() const;
    
    /*
     * The name of this function.
     */
    std::string function_name() const;
    
    /*
     * The list of parameters for this function.
     */
    std::vector<FormalParameterNode*> parameters() const;
    
    /*
     * Whether the function's return type is a struct type or not.
     */
    bool is_struct() const;
    
    /*
     * Whether the function's return type is a constant type or not.
     */
    bool is_const() const;

};

}

#endif