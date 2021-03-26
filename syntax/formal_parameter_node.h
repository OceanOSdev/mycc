#ifndef SYNTAX_FORMAL_PARAMETER_NODE_H
#define SYNTAX_FORMAL_PARAMETER_NODE_H

#include <string>
#include "syntax_node.h"

namespace Syntax {

class FormalParameterNode : public SyntaxNode {
private:
    std::string m_type_identifier;
    std::string m_param_name;
    bool m_is_struct;
    bool m_is_const;
    bool m_is_array;
public:
    FormalParameterNode(std::string id, 
                        std::string name,
                        bool is_struct = false,
                        bool is_const = false,
                        bool is_array = false) :
        m_type_identifier(id),
        m_param_name(name),
        m_is_struct(is_struct),
        m_is_const(is_const),
        m_is_array(is_array) {}
    
    ~FormalParameterNode() {}

    /*
     * The name of the type that the parameter is.
     */
    std::string type_identifier() const { return m_type_identifier; }
    
    /*
     * The name of the parameter.
     */
    std::string param_name() const { return m_param_name; }
    
    /*
     * Whether the parameter is a struct type or not.
     */
    bool is_struct() const { return m_is_struct; }

    /*
     * Whether the parameter is a constant type or not.
     */
    bool is_const() const { return m_is_const; }

    /*
     * Whether the parameter is an array or not.
     */
    bool is_array() const { return m_is_array; }

};

}

#endif