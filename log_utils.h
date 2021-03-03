#ifndef LOG_UTILS_H
#define LOG_UTILS_H
#include "mycc.tab.h"
/*
 * Logs the current file name, the line number, the text, and the token
 * in the form:
 * 
 * "FILENAME line LINENUMBER text 'TEXT' token TOKENNAME"
 * 
 * To the output stream.
 */
void log_lex_info(char* filename, int lineNum, char* text, enum yytokentype token);

/*
 * Logs an error that occured in lexing provided a description, the file name,
 * the line number, and optionally the text in the form:
 * 
 * "Error ERROR_DESCRIPTION file FILENAME line LINENUMBER (text 'TEXT')"
 * 
 * To the output stream. (Usually stderr)
 */
void log_lex_err(char* errDesc, char* filename, int lineNum, char* text);

#endif