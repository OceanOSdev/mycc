#include "formal_parameter_node.h"

namespace Syntax {

FormalParameterNode::FormalParameterNode(std::string id, 
                    std::string name,
                    bool is_struct,
                    bool is_const,
                    bool is_array) :
    m_type_identifier(id),
    m_param_name(name),
    m_is_struct(is_struct),
    m_is_const(is_const),
    m_is_array(is_array) {}

FormalParameterNode::~FormalParameterNode() {}

/*
 * The name of the type that the parameter is.
 */
std::string FormalParameterNode::type_identifier() const { 
    return m_type_identifier; 
}

    
/*
 * The name of the parameter.
 */
std::string FormalParameterNode::param_name() const { 
    return m_param_name; 
}

    
/*
 * Whether the parameter is a struct type or not.
 */
bool FormalParameterNode::is_struct() const { 
    return m_is_struct; 
}

/*
 * Whether the parameter is a constant type or not.
 */
bool FormalParameterNode::is_const() const { 
    return m_is_const; 
}

/*
 * Whether the parameter is an array or not.
 */
bool FormalParameterNode::is_array() const { 
    return m_is_array; 
}


}