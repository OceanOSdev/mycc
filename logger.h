#ifndef LOGGER_H
#define LOGGER_H
#include "mycc.tab.hpp"
#include "lexeme_data.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

typedef enum yy::parser::token::yytokentype token_type;

class Logger {
private:
    std::ostream* m_outstream;
    bool m_is_stdio;

public:
    /*
     * Initializes a logger with default output stream of cout
     */
    Logger() : m_outstream(&std::cout), m_is_stdio(true) {}

    /*
     * Initializes a logger with output to file "filename"
     */
    Logger(std::string filename) : m_outstream(new std::ofstream(filename)), m_is_stdio(false) {}

    ~Logger();

    void log_info(std::string str);

    /*
    * Logs the current file name, the line number, the text, and the token
    * in the form:
    * 
    * "FILENAME line LINENUMBER text 'TEXT' token TOKENNAME"
    */
    void log_lex_info(char* filename, int lineNum, char* text, token_type token);

    /*
    * Logs the current file name, the line number, the text, and the token
    * in the form:
    * 
    * "FILENAME line LINENUMBER text 'TEXT' token TOKENNAME"
    */
    void log_lex_info_v2(LexemeDataNode lexeme_data);

    /*
    * Logs an error that occured in lexing provided a description, the file name,
    * the line number, and optionally the text in the form:
    * 
    * "Error ERROR_DESCRIPTION file FILENAME line LINENUMBER (text 'TEXT')"
    */
    void log_lex_err(char* errDesc, char* filename, int lineNum, char* text);

    /*
    * Logs an error that occured in lexing provided a description, the file name,
    * the line number, and optionally the text in the form:
    * 
    * "Error ERROR_DESCRIPTION file FILENAME line LINENUMBER (text 'TEXT')"
    */
    void log_lex_err_v2(LexemeDataNode lexeme_data);

    /*
    * Logs the info related to a global struct definition symbol
    * as generated from the parser.
    */
    //void log_parser_glob_struct_symbol(FILE* fout, struct_decl_symbol_t* sds);

    /*
    * Logs the info related to a function declaration symbol
    * as generated from the parser.
    */
    //void log_parser_func_decl_symbol(FILE* fout, func_decl_symbol_t* fds);

    /*
    * Logs the info related to a function prototype symbol
    * as generated from the parser.
    */
    //void log_parser_func_proto_symbol(FILE* fout, func_proto_symbol_t* fps);

    /*
    * Logs an error that has been thrown by the parser in yyerror.
    */
    //void log_parser_error(FILE* fout, parse_error_symbol_t* pes);

    /*
    * Logs a list of strings such that they are comma delimited.
    */
    void log_string_list(char* argv[]);
};

#endif