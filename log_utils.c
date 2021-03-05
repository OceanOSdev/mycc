#include "log_utils.h"

char* token_to_string(enum yytokentype token);
void log_string_list(FILE* fout, char* argv[]);
void log_struct_names(FILE* fout, struct_decl_symbol_t** structs);

void log_lex_info(FILE* fout, char* filename, int lineNum, char* text, enum yytokentype token) {
    fprintf(fout, "%s line %d text \'%s\' token %s\n", filename, lineNum, text, token_to_string(token));
}

void log_lex_err(FILE* fout, char* errDesc, char* filename, int lineNum, char* text) {
    FILE* ostream = fout != stdout ? fout : stderr;
    fprintf(ostream, "Error %s file %s line %d", errDesc, filename, lineNum);
    if (text)
        fprintf(ostream, " text \'%s\'", text);
    fprintf(ostream, "\n");
}

void log_parser_glob_struct_symbol(FILE* fout, struct_decl_symbol_t* sds) {
    fprintf(fout, "Global struct %s", sds->struct_name);
    if (sds->num_members > 0) {
        fprintf(fout, "\n\t");
        log_string_list(fout, sds->struct_members);
    }
    fprintf(fout, "\n");
}

void log_parser_func_decl_symbol(FILE* fout, func_decl_symbol_t* fds) {
    fprintf(fout, "Function %s", fds->func_name);
    if (fds->num_params > 0) {
        fprintf(fout, "\n\tParameters: ");
        log_string_list(fout, fds->func_params);
    }
    if (fds->num_structs > 0) {
        fprintf(fout, "\n\tLocal structs: ");
        log_struct_names(fout, fds->structs);
    }
    if (fds->num_lvars > 0) {
        fprintf(fout, "\n\tLocal variables: ");
        log_string_list(fout, fds->func_local_vars);
    }
    fprintf(fout, "\n");
}

void log_parser_func_proto_symbol(FILE* fout, func_proto_symbol_t* fps) {
    fprintf(fout, "Prototype %s", fps->func_name);
    if (fps->num_params > 0) {
        fprintf(fout, "\n\tParameters: ");
        log_string_list(fout, fps->func_params);
    }
    fprintf(fout, "\n");
}

void log_parser_error(FILE* fout, parse_error_symbol_t* pes) {
    FILE* ostream = fout != stdout ? fout : stderr;
    fprintf(ostream, "Error near %s line %d text '%s'\n\t%s\n", 
        pes->filename, pes->line_number, pes->text, pes->msg);
}

void log_string_list(FILE* fout, char* argv[]) {
    int i = 0;
    while (argv[i]) {
        fprintf(fout, "%s", argv[i]);
        if (argv[++i]) fprintf(fout,",");
        fprintf(fout, " ");
    }
}

void log_struct_names(FILE* fout, struct_decl_symbol_t** structs) {
    int i = 0;
    while (structs[i]) {
        fprintf(fout, "%s", structs[i]->struct_name);
        if (structs[++i]) fprintf(fout,",");
        fprintf(fout, " ");
    }
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
