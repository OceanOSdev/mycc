#pragma once
#ifndef BOUND_LITERAL_VAL_EXPRESSION_NODE_H
#define BOUND_LITERAL_VAL_EXPRESSION_NODE_H

#include "bound_expression_node.h"
#include <variant>
#include <string>

namespace Binding {

class BoundLiteralValExpressionNode : public BoundExpressionNode {
private:
    std::variant<int, float, char, std::string> m_value;
    const Symbols::TypeSymbol* m_type;
public:
    BoundLiteralValExpressionNode(int value);
    BoundLiteralValExpressionNode(float value);
    BoundLiteralValExpressionNode(char value);
    BoundLiteralValExpressionNode(std::string value);

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

    /*
     * The type that this expression evaluates to.
     */
    const Symbols::TypeSymbol* type() const override;

    /*
     * An enum representing the kind of bound node this is
     */
    BoundNodeKind kind() const override;
};

}

#endif