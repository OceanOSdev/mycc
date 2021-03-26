#ifndef SYNTAX_LITERAL_VAL_EXPRESSION_NODE_H
#define SYNTAX_LITERAL_VAL_EXPRESSION_NODE_H

#include <string>
#include "expression_node.h"

namespace Syntax {

typedef enum literal_val_type {
    INTCONST,
    REALCONST,
    STRCONST,
    CHARCONST
} LiteralValType;


typedef struct literal_val_wrapper {
    int i_val;
    float f_val;
    std::string s_val;
    char c_val;
} LiteralValWrapper;


class LiteralValExpressionNode : public ExpressionNode {
private:
    LiteralValType v_type;
    LiteralValWrapper v_wrapper;
public:
    LiteralValExpressionNode(LiteralValType type, LiteralValWrapper value) :
        v_type(type), v_wrapper(value) {}
    
    ~LiteralValExpressionNode() { }

    /*
     * An enum value representing the type stored in this node.
     */
    LiteralValType value_type() const { return v_type; }

    /*
     * The int value (if the type stored is an int).
     */
    int int_value() const { return v_wrapper.i_val; }

    /*
     * The float value (if the type stored is a float).
     */
    float float_value() const { return v_wrapper.f_val; }

    /*
     * The string value (if the type stored is a string).
     */
    std::string string_value() const { return v_wrapper.s_val; }

    /*
     * The char value (if the type stored is a char).
     */
    char char_value() const { return v_wrapper.c_val; }
};

}

#endif