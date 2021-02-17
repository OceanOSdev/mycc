#ifndef TOKENS_H
#define TOKENS_H

typedef enum token {
    TYPE = 260,
    CONST,
    STRUCT,
    FOR,
    WHILE,
    DO,
    IF,
    ELSE,
    BREAK,
    CONTINUE,
    RETURN,
    IDENT,
    INTCONST,
    REALCONST,
    STRCONST,
    CHARCONST,

    LPAR,
    RPAR,
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,

    DOT,
    COMMA,
    SEMI,
    QUEST,
    COLON,

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
} token_e;

#endif