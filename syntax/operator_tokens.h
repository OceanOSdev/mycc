#ifndef SYNTAX_OPERATOR_TOKENS_H
#define SYNTAX_OPERATOR_TOKENS_H

namespace Syntax {
    typedef enum syntax_token_type {
        PLUS,
        MINUS, 
        STAR, 
        SLASH, 
        MOD, 
        TILDE,
        PIPE,
        AMP,
        BANG,
        DPIPE,
        DAMP,
        ASSIGN,
        PLUSASSIGN,
        MINUSASSIGN,
        STARASSIGN,
        SLASHASSIGN,
        INCR,
        DECR,
        EQUALS,
        NEQUAL,
        GT,
        GE,
        LT,
        LE
    } SyntaxTokenType;
}

#endif