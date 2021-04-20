#pragma once
#ifndef SYNTAX_LITERAL_VAL_EXPRESSION_NODE_H
#define SYNTAX_LITERAL_VAL_EXPRESSION_NODE_H

#include <string>
#include "expression_node.h"

namespace Syntax {
enum TOKEN_DATA_TYPE : int;

class LiteralValExpressionNode : public ExpressionNode {
public:
    LiteralValExpressionNode(SyntaxToken* token);
    
    ~LiteralValExpressionNode();

    /*
     * An enum value representing the type stored in this node.
     */
    enum TOKEN_DATA_TYPE value_type() const;

    /*
     * The int value (if the type stored is an int).
     */
    int int_value() const;

    /*
     * The float value (if the type stored is a float).
     */
    float float_value() const;

    /*
     * The string value (if the type stored is a string).
     */
    std::string string_value() const;

    /*
     * The char value (if the type stored is a char).
     */
    char char_value() const;

    SyntaxKind kind() const override;
};

}

#endif