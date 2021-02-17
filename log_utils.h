#ifndef LOG_UTILS_H
#define LOG_UTILS_H
#include "tokens.h"

void log_lex_info(char* filename, int lineNum, char* text, token_e token);
void log_lex_err(char* errDesc, char* filename, int lineNum, char* text);

#endif