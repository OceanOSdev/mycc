%{
#include <math.h>
#include <stdio.h>
#include "tokens.h"
#include "log_utils.h"
extern char** input_comp_files;

%}

NUMBER [0-9]
ID     [a-zA-Z_][a-zA-Z0-9_]*
FNAME  [a-zA-Z0-9_]*\.[a-zA-Z0-9_]+

%x COM
%option yylineno
%option noyywrap

%%

[ \t\r\n]*                      { /* ignore */ }
"/*"                            { BEGIN(COM); }
<COM>"*/"                       { BEGIN(INITIAL); }
<COM>[^*\n]+                    { /* eat comment in chunks */ }
<COM>"*"                        { /* eat the lone star */ }
<COM>\n                         { yylineno++; }
"//.*\n"                        { /* ignore */ }
"#include "\"{FNAME}\"          { /* ignore */ }
"#define "{ID}.*                { /* ignore */ }
"#undef "{ID}                   { /* ignore */ }
"#ifdef "{ID}                   { /* ignore */ }
"#ifndef "{ID}                  { /* ignore */ }
"#else"                         { /* ignore */ }
"#endif"                        { /* ignore */ }
"void"|"char"|"int"|"float"     { log_lex_info(*input_comp_files,yylineno,yytext,TYPE); }
"const"                         { log_lex_info(*input_comp_files,yylineno,yytext,CONST); } 
"struct"                        { log_lex_info(*input_comp_files,yylineno,yytext,STRUCT); }        
"for"                           { log_lex_info(*input_comp_files,yylineno,yytext,FOR); }   
"while"                         { log_lex_info(*input_comp_files,yylineno,yytext,WHILE); } 
"do"                            { log_lex_info(*input_comp_files,yylineno,yytext,DO); }    
"if"                            { log_lex_info(*input_comp_files,yylineno,yytext,IF); }    
"else"                          { log_lex_info(*input_comp_files,yylineno,yytext,ELSE); }  
"break"                         { log_lex_info(*input_comp_files,yylineno,yytext,BREAK); } 
"continue"                      { log_lex_info(*input_comp_files,yylineno,yytext,CONTINUE); }      
"return"                        { log_lex_info(*input_comp_files,yylineno,yytext,RETURN); }
{NUMBER}+                       { log_lex_info(*input_comp_files,yylineno,yytext,INTCONST); }
{NUMBER}+"."?{NUMBER}           { log_lex_info(*input_comp_files,yylineno,yytext,REALCONST); }
\"(\\.|[^"\\])*\"               { log_lex_info(*input_comp_files,yylineno,yytext,STRCONST); }
\'([^\\]|\\[abnrt\'\\])\'       { log_lex_info(*input_comp_files,yylineno,yytext,CHARCONST); }
"("                             { log_lex_info(*input_comp_files,yylineno,yytext,LPAR); }
")"                             { log_lex_info(*input_comp_files,yylineno,yytext,RPAR); }
"["                             { log_lex_info(*input_comp_files,yylineno,yytext,LBRACKET); }
"]"                             { log_lex_info(*input_comp_files,yylineno,yytext,RBRACKET); }
"{"                             { log_lex_info(*input_comp_files,yylineno,yytext,LBRACE); }
"}"                             { log_lex_info(*input_comp_files,yylineno,yytext,RBRACE); }
"."                             { log_lex_info(*input_comp_files,yylineno,yytext,DOT); }
","                             { log_lex_info(*input_comp_files,yylineno,yytext,COMMA); }
";"                             { log_lex_info(*input_comp_files,yylineno,yytext,SEMI); }
"?"                             { log_lex_info(*input_comp_files,yylineno,yytext,QUEST); }
":"                             { log_lex_info(*input_comp_files,yylineno,yytext,COLON); }
"+"                             { log_lex_info(*input_comp_files,yylineno,yytext,PLUS); }
"-"                             { log_lex_info(*input_comp_files,yylineno,yytext,MINUS); }
"*"                             { log_lex_info(*input_comp_files,yylineno,yytext,STAR); }
"/"                             { log_lex_info(*input_comp_files,yylineno,yytext,SLASH); }
"%"                             { log_lex_info(*input_comp_files,yylineno,yytext,MOD); }
"~"                             { log_lex_info(*input_comp_files,yylineno,yytext,TILDE); }
"|"                             { log_lex_info(*input_comp_files,yylineno,yytext,PIPE); }
"&"                             { log_lex_info(*input_comp_files,yylineno,yytext,AMP); }
"!"                             { log_lex_info(*input_comp_files,yylineno,yytext,BANG); }
"||"                            { log_lex_info(*input_comp_files,yylineno,yytext,DPIPE); }
"&&"                            { log_lex_info(*input_comp_files,yylineno,yytext,DAMP); }
"="                             { log_lex_info(*input_comp_files,yylineno,yytext,ASSIGN); }
"+="                            { log_lex_info(*input_comp_files,yylineno,yytext,PLUSASSIGN); }
"-="                            { log_lex_info(*input_comp_files,yylineno,yytext,MINUSASSIGN); }
"*="                            { log_lex_info(*input_comp_files,yylineno,yytext,STARASSIGN); }
"/="                            { log_lex_info(*input_comp_files,yylineno,yytext,SLASHASSIGN); }
"++"                            { log_lex_info(*input_comp_files,yylineno,yytext,INCR); }
"--"                            { log_lex_info(*input_comp_files,yylineno,yytext,DECR); }
"=="                            { log_lex_info(*input_comp_files,yylineno,yytext,EQUALS); }
"!="                            { log_lex_info(*input_comp_files,yylineno,yytext,NEQUAL); }
">"                             { log_lex_info(*input_comp_files,yylineno,yytext,GT); }
">="                            { log_lex_info(*input_comp_files,yylineno,yytext,GE); }
"<"                             { log_lex_info(*input_comp_files,yylineno,yytext,LT); }
"<="                            { log_lex_info(*input_comp_files,yylineno,yytext,LE); }
{ID}                            { log_lex_info(*input_comp_files,yylineno,yytext,IDENT); }
.                               { log_lex_err("Unexpected symbol",*input_comp_files,yylineno,yytext); }
<<EOF>>                         {if(*++input_comp_files) yyin = fopen( *input_comp_files, "r" ); else yyterminate();}
%%

void initfile(char* filename) {
    yyin = fopen( filename, "r" );
}