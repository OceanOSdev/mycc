#pragma once
#ifndef SYNTAX_INDEX_EXPRESSION_NODE_H
#define SYNTAX_INDEX_EXPRESSION_NODE_H

#include <string>
#include "expression_node.h"

namespace Syntax {

class IndexExpressionNode : public ExpressionNode {
private:
    std::string identifier;
    ExpressionNode* expr;
public:
    IndexExpressionNode(std::string id, ExpressionNode* e);

    ~IndexExpressionNode();

    /*
     * The name of the identifier (as an l_val).
     */
    std::string name() const;

    /*
     * The expression that evaluates to the index.
     */
    ExpressionNode* expression() const;
};

}

#endif