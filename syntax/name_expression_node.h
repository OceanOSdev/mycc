#pragma once
#ifndef SYNTAX_NAME_EXPRESSION_NODE_H
#define SYNTAX_NAME_EXPRESSION_NODE_H

#include <string>
#include "expression_node.h"

namespace Syntax {

class NameExpressionNode : public ExpressionNode {
private:
    std::string identifier;
public:
    NameExpressionNode(SyntaxToken* token, std::string id);

    ~NameExpressionNode();

    /*
     * The name of the identifier (as an l_val).
     */
    std::string name() const;
    
    SyntaxKind kind() const override;
};

}

#endif