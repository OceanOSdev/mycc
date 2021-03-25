#ifndef SYNTAX_CAST_EXPRESSION_NODE_H
#define SYNTAX_CAST_EXPRESSION_NODE_H

#include <string>
#include "expression_node.h"

namespace Syntax {

class CastExpressionNode : public ExpressionNode {
private:
    std::string casting_type;
    ExpressionNode* expression_node;
public:
    CastExpressionNode(std::string type, ExpressionNode* expr) :
        casting_type(type), expression_node(expr) {}

    ~CastExpressionNode() {
        delete expression_node;
    }

    /*
     * The type that the expression r-value gets cast to.
     */
    std::string type() const { return casting_type; }
    
    /*
     * The expression being casted.
     */
    ExpressionNode* expression() const { return expression_node; }
};

}

#endif