#include "name_expression_node.h"

namespace Syntax {

NameExpressionNode::NameExpressionNode(std::string id) : 
    identifier(id) {}

NameExpressionNode::~NameExpressionNode() {}

/*
 * The name of the identifier (as an l_val).
 */
std::string NameExpressionNode::name() const { 
    return identifier; 
}

}