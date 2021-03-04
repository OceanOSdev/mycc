#ifndef LOG_UTILS_H
#define LOG_UTILS_H
#include "mycc.tab.h"
#include "crude_symbols_list.h"
#include <stdio.h>
/*
 * Logs the current file name, the line number, the text, and the token
 * in the form:
 * 
 * "FILENAME line LINENUMBER text 'TEXT' token TOKENNAME"
 */
void log_lex_info(FILE* fout, char* filename, int lineNum, char* text, enum yytokentype token);

/*
 * Logs an error that occured in lexing provided a description, the file name,
 * the line number, and optionally the text in the form:
 * 
 * "Error ERROR_DESCRIPTION file FILENAME line LINENUMBER (text 'TEXT')"
 */
void log_lex_err(FILE* fout, char* errDesc, char* filename, int lineNum, char* text);

/*
 * Logs the info related to a global struct definition symbol
 * as generated from the parser.
 */
void log_parser_glob_struct_symbol(FILE* fout, struct_decl_symbol_t* sds);

/*
 * Logs the info related to a function declaration symbol
 * as generated from the parser.
 */
void log_parser_func_decl_symbol(FILE* fout, func_decl_symbol_t* fds);

/*
 * Logs the info related to a function prototype symbol
 * as generated from the parser.
 */
void log_parser_func_proto_symbol(FILE* fout, func_proto_symbol_t* fps);

/*
 * Logs an error that has been thrown by the parser in yyerror.
 */
void log_parser_error(FILE* fout, parse_error_symbol_t* pes);

/*
 * Logs a list of strings such that they are comma delimited.
 */
void log_string_list(FILE* fout, char* argv[]);

#endif