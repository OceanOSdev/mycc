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

    /*
     * Logs string as info.
     */
    void log_info(std::string str);

    /*
     * Logs string as info with newline at end.
     */
    void log_info_nl(std::string str);

    /*
     * Logs a string as an error.
     */
    void log_err(std::string err);

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
    * Logs a list of strings such that they are comma delimited.
    */
    void log_string_list(char* argv[]);
};

#endif