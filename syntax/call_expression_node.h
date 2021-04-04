#pragma once
#ifndef SYNTAX_CALL_EXPRESSION_NODE_H
#define SYNTAX_CALL_EXPRESSION_NODE_H

#include <string>
#include <vector>
#include "expression_node.h"

namespace Syntax {

class CallExpressionNode : public ExpressionNode {
private:
    std::string identifier;
    std::vector<ExpressionNode*> expression_list;
public:
    CallExpressionNode(std::string id, std::vector<ExpressionNode*> expr_list);
    
    ~CallExpressionNode();

    /*
     * The name of the method (lets be real thats whats getting called) 
     * being called.
     */
    std::string name() const;

    /*
     * The list of expressions being passed as arguments.
     */
    std::vector<ExpressionNode*> expressions() const;
};

}

#endif