#include "index_expression_node.h"

namespace Syntax {

IndexExpressionNode::IndexExpressionNode(std::string id, ExpressionNode* e) : 
    identifier(id), expr(e) {}

IndexExpressionNode::~IndexExpressionNode() { 
    delete expr; 
}

/*
 * The name of the identifier (as an l_val).
 */
std::string IndexExpressionNode::name() const { 
    return identifier; 
}

/*
 * The expression that evaluates to the index.
 */
ExpressionNode* IndexExpressionNode::expression() const { 
    return expr; 
}


}