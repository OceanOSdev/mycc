#include <stdio.h>
#include "log_utils.h"

char* token_to_string(enum yytokentype token);

void log_lex_info(char* filename, int lineNum, char* text, enum yytokentype token) {
    printf("%s line %d text \'%s\' token %s\n", filename, lineNum, text, token_to_string(token));
}

void log_lex_err(char* errDesc, char* filename, int lineNum, char* text) {
    fprintf(stderr, "Error %s file %s line %d", errDesc, filename, lineNum);
    if (text)
        fprintf(stderr, " text \'%s\'", text);
    fprintf(stderr, "\n");
}


char* token_to_string(enum yytokentype token) {
    switch (token) {
        case TYPE:
            return "TYPE";
        case CONST:
            return "CONST";
        case STRUCT:
            return "STRUCT";
        case FOR:
            return "FOR";
        case WHILE:
            return "WHILE";
        case DO:
            return "DO";
        case IF:
            return "IF";
        case ELSE:
            return "ELSE";
        case BREAK:
            return "BREAK";
        case CONTINUE:
            return "CONTINUE";
        case RETURN:
            return "RETURN";
        case IDENT:
            return "IDENT";
        case INTCONST:
            return "INTCONST";
        case REALCONST:
            return "REALCONST";
        case STRCONST:
            return "STRCONST";
        case CHARCONST:
            return "CHARCONST";
        case LPAR:
            return "LPAR";
        case RPAR:
            return "RPAR";
        case LBRACKET:
            return "LBRACKET";
        case RBRACKET:
            return "RBRACKET";
        case LBRACE:
            return "LBRACE";
        case RBRACE:
            return "RBRACE";
        case DOT:
            return "DOT";
        case COMMA:
            return "COMMA";
        case SEMI:
            return "SEMI";
        case QUEST:
            return "QUEST";
        case COLON:
            return "COLON";
        case PLUS:
            return "PLUS";
        case MINUS:
            return "MINUS";
        case STAR:
            return "STAR";
        case SLASH:
            return "SLASH";
        case MOD:
            return "MOD";
        case TILDE:
            return "TILDE";
        case PIPE:
            return "PIPE";
        case AMP:
            return "AMP";
        case BANG:
            return "BANG";
        case DPIPE:
            return "DPIPE";
        case DAMP:
            return "DAMP";
        case ASSIGN:
            return "ASSIGN";
        case PLUSASSIGN:
            return "PLUSASSIGN";
        case MINUSASSIGN:
            return "MINUSASSIGN";
        case STARASSIGN:
            return "STARASSIGN";
        case SLASHASSIGN:
            return "SLASHASSIGN";
        case INCR:
            return "INCR";
        case DECR:
            return "DECR";
        case EQUALS:
            return "EQUALS";
        case NEQUAL:
            return "NEQUAL";
        case GT:
            return "GT";
        case GE:
            return "GE";
        case LT:
            return "LT";
        case LE:
            return "LE";
    }
}
