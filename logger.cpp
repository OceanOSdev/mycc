#include "logger.h"

Logger::~Logger() {
    if (!m_is_stdio) {
        delete m_outstream;
    }
}

void Logger::log_info_nl(std::string str) {
    (*m_outstream) << str << std::endl;
}

void Logger::log_info(std::string str) {
    (*m_outstream) << str;
}

std::string token_to_string(token_type token);
void log_string_list(std::ostream& fout, char* argv[]);
//void log_struct_names(ofstream fout, struct_decl_symbol_t** structs);

void Logger::log_lex_info(char* filename, int lineNum, char* text, token_type token) {
    (*m_outstream) << filename << " line " << lineNum << " '" << text << "' token " << token_to_string(token) << std::endl;
    //fprintf(fout, "%s line %d text \'%s\' token %s\n", filename, lineNum, text, token_to_string(token));
}


void Logger::log_lex_info_v2(LexemeDataNode lexeme_data) {
    yy::parser::symbol_type symbol_info = lexeme_data.symbol_info;
    std::string filename = *(symbol_info.location.begin.filename);
    yy::position::counter_type bline = symbol_info.location.begin.line;
    std::string token_name = token_to_string(lexeme_data.token);
    (*m_outstream) << filename << " line " << bline << " text '" << lexeme_data.text << "' token " << token_name << std::endl;
}

void Logger::log_lex_err( char* errDesc, char* filename, int lineNum, char* text) {
    *(m_is_stdio ? &std::cerr : m_outstream) << "Error " << errDesc << " file " << filename << "line " << lineNum;    
    //fprintf(ostream, "Error %s file %s line %d", errDesc, filename, lineNum);
    if (text)
        *(m_is_stdio ? &std::cerr : m_outstream) << " text '" << text << "'"; //fprintf(ostream, " text \'%s\'", text);
    *(m_is_stdio ? &std::cerr : m_outstream) << std::endl; //fprintf(ostream, "\n");
}

void Logger::log_lex_err_v2(LexemeDataNode lexeme_data) {
    std::string filename = *(lexeme_data.symbol_info.location.begin.filename);
    yy::position::counter_type line_num = lexeme_data.symbol_info.location.begin.line;
    *(m_is_stdio ? &std::cerr : m_outstream) << "Error " << lexeme_data.err_desc << " file " << filename << " line " << line_num;
    if (!lexeme_data.text.empty())
        *(m_is_stdio ? &std::cerr : m_outstream) << " text '" << lexeme_data.text << "'";
    *(m_is_stdio ? &std::cerr : m_outstream) << std::endl;
}

// void log_parser_glob_struct_symbol(FILE* fout, struct_decl_symbol_t* sds) {
//     fprintf(fout, "Global struct %s", sds->struct_name);
//     if (sds->num_members > 0) {
//         fprintf(fout, "\n\t");
//         log_string_list(fout, sds->struct_members);
//     }
//     fprintf(fout, "\n");
// }

// void log_parser_func_decl_symbol(FILE* fout, func_decl_symbol_t* fds) {
//     fprintf(fout, "Function %s", fds->func_name);
//     if (fds->num_params > 0) {
//         fprintf(fout, "\n\tParameters: ");
//         log_string_list(fout, fds->func_params);
//     }
//     if (fds->num_structs > 0) {
//         fprintf(fout, "\n\tLocal structs: ");
//         log_struct_names(fout, fds->structs);
//     }
//     if (fds->num_lvars > 0) {
//         fprintf(fout, "\n\tLocal variables: ");
//         log_string_list(fout, fds->func_local_vars);
//     }
//     fprintf(fout, "\n");
// }

// void log_parser_func_proto_symbol(FILE* fout, func_proto_symbol_t* fps) {
//     fprintf(fout, "Prototype %s", fps->func_name);
//     if (fps->num_params > 0) {
//         fprintf(fout, "\n\tParameters: ");
//         log_string_list(fout, fps->func_params);
//     }
//     fprintf(fout, "\n");
// }

// void log_parser_error(FILE* fout, parse_error_symbol_t* pes) {
//     FILE* ostream = fout != stdout ? fout : stderr;
//     fprintf(ostream, "Error near %s line %d text '%s'\n\t%s\n", 
//         pes->filename, pes->line_number, pes->text, pes->msg);
// }

// void log_struct_names(FILE* fout, struct_decl_symbol_t** structs) {
//     int i = 0;
//     while (structs[i]) {
//         fprintf(fout, "%s", structs[i]->struct_name);
//         if (structs[++i]) fprintf(fout,",");
//         fprintf(fout, " ");
//     }
// }

std::string token_to_string(token_type token) {
    switch (token) {
        case token_type::TYPE: 
            return "TYPE";
        case token_type::CONST:
            return "CONST";
        case token_type::STRUCT:
            return "STRUCT";
        case token_type::FOR:
            return "FOR";
        case token_type::WHILE:
            return "WHILE";
        case token_type::DO:
            return "DO";
        case token_type::IF:
            return "IF";
        case token_type::ELSE:
            return "ELSE";
        case token_type::BREAK:
            return "BREAK";
        case token_type::CONTINUE:
            return "CONTINUE";
        case token_type::RETURN:
            return "RETURN";
        case token_type::IDENT:
            return "IDENT";
        case token_type::INTCONST:
            return "INTCONST";
        case token_type::REALCONST:
            return "REALCONST";
        case token_type::STRCONST:
            return "STRCONST";
        case token_type::CHARCONST:
            return "CHARCONST";
        case token_type::LPAR:
            return "LPAR";
        case token_type::RPAR:
            return "RPAR";
        case token_type::LBRACKET:
            return "LBRACKET";
        case token_type::RBRACKET:
            return "RBRACKET";
        case token_type::LBRACE:
            return "LBRACE";
        case token_type::RBRACE:
            return "RBRACE";
        case token_type::DOT:
            return "DOT";
        case token_type::COMMA:
            return "COMMA";
        case token_type::SEMI:
            return "SEMI";
        case token_type::QUEST:
            return "QUEST";
        case token_type::COLON:
            return "COLON";
        case token_type::PLUS:
            return "PLUS";
        case token_type::MINUS:
            return "MINUS";
        case token_type::STAR:
            return "STAR";
        case token_type::SLASH:
            return "SLASH";
        case token_type::MOD:
            return "MOD";
        case token_type::TILDE:
            return "TILDE";
        case token_type::PIPE:
            return "PIPE";
        case token_type::AMP:
            return "AMP";
        case token_type::BANG:
            return "BANG";
        case token_type::DPIPE:
            return "DPIPE";
        case token_type::DAMP:
            return "DAMP";
        case token_type::ASSIGN:
            return "ASSIGN";
        case token_type::PLUSASSIGN:
            return "PLUSASSIGN";
        case token_type::MINUSASSIGN:
            return "MINUSASSIGN";
        case token_type::STARASSIGN:
            return "STARASSIGN";
        case token_type::SLASHASSIGN:
            return "SLASHASSIGN";
        case token_type::INCR:
            return "INCR";
        case token_type::DECR:
            return "DECR";
        case token_type::EQUALS:
            return "EQUALS";
        case token_type::NEQUAL:
            return "NEQUAL";
        case token_type::GT:
            return "GT";
        case token_type::GE:
            return "GE";
        case token_type::LT:
            return "LT";
        case token_type::LE:
            return "LE";
        default:
            return "UNKNOWN"; //shouldn't ever reach here
    }
}
