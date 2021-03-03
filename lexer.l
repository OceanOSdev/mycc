%{
#include <math.h>
#include <stdio.h>
#include "log_utils.h"
#include "mycc.tab.h" // bison generated tokens
extern char** input_comp_files;
int openingCommentLine = 0;
%}

NUMBER [0-9]
ID     [a-zA-Z_][a-zA-Z0-9_]*
FNAME  [a-zA-Z0-9_]*\.[a-zA-Z0-9_]+

%x COM
%option yylineno
%option noyywrap

%%

[ \t\r\n]*                      { /* ignore */ }
"/*"                            { BEGIN(COM); openingCommentLine = yylineno; }
<COM>"*/"                       { BEGIN(INITIAL); openingCommentLine = 0; }
<COM>[^*\n]+                    { /* eat comment in chunks */ }
<COM>"*"                        { /* eat the lone star */ }
<COM>\n                         { /* ignore */ }
"//".*                          { /* ignore */ }
"#include "\"{FNAME}\"          { /* ignore */ }
"#define "{ID}.*                { /* ignore */ }
"#undef "{ID}                   { /* ignore */ }
"#ifdef "{ID}                   { /* ignore */ }
"#ifndef "{ID}                  { /* ignore */ }
"#else"                         { /* ignore */ }
"#endif"                        { /* ignore */ }
"void"|"char"|"int"|"float"     { log_lex_info(*input_comp_files,yylineno,yytext,TYPE); return TYPE; }
"const"                         { log_lex_info(*input_comp_files,yylineno,yytext,CONST); return CONST; } 
"struct"                        { log_lex_info(*input_comp_files,yylineno,yytext,STRUCT); return STRUCT; }        
"for"                           { log_lex_info(*input_comp_files,yylineno,yytext,FOR); return FOR; }   
"while"                         { log_lex_info(*input_comp_files,yylineno,yytext,WHILE); return WHILE; } 
"do"                            { log_lex_info(*input_comp_files,yylineno,yytext,DO); return DO; }    
"if"                            { log_lex_info(*input_comp_files,yylineno,yytext,IF); return IF; }    
"else"                          { log_lex_info(*input_comp_files,yylineno,yytext,ELSE); return ELSE; }  
"break"                         { log_lex_info(*input_comp_files,yylineno,yytext,BREAK); return BREAK; } 
"continue"                      { log_lex_info(*input_comp_files,yylineno,yytext,CONTINUE); return CONTINUE; }      
"return"                        { log_lex_info(*input_comp_files,yylineno,yytext,RETURN); return RETURN; }
{NUMBER}+                       { log_lex_info(*input_comp_files,yylineno,yytext,INTCONST); return INTCONST; }
{NUMBER}+"."?{NUMBER}           { log_lex_info(*input_comp_files,yylineno,yytext,REALCONST); return REALCONST; }
\"(\\.|[^"\\])*\"               { log_lex_info(*input_comp_files,yylineno,yytext,STRCONST); return STRCONST; }
\'([^\\]|\\[abnrt\'\\])\'       { log_lex_info(*input_comp_files,yylineno,yytext,CHARCONST); return CHARCONST; }
"("                             { log_lex_info(*input_comp_files,yylineno,yytext,LPAR); return LPAR; }
")"                             { log_lex_info(*input_comp_files,yylineno,yytext,RPAR); return RPAR; }
"["                             { log_lex_info(*input_comp_files,yylineno,yytext,LBRACKET); return LBRACKET; }
"]"                             { log_lex_info(*input_comp_files,yylineno,yytext,RBRACKET); return RBRACKET; }
"{"                             { log_lex_info(*input_comp_files,yylineno,yytext,LBRACE); return LBRACE; }
"}"                             { log_lex_info(*input_comp_files,yylineno,yytext,RBRACE); return RBRACE; }
"."                             { log_lex_info(*input_comp_files,yylineno,yytext,DOT); return DOT; }
","                             { log_lex_info(*input_comp_files,yylineno,yytext,COMMA); return COMMA; }
";"                             { log_lex_info(*input_comp_files,yylineno,yytext,SEMI); return SEMI; }
"?"                             { log_lex_info(*input_comp_files,yylineno,yytext,QUEST); return QUEST; }
":"                             { log_lex_info(*input_comp_files,yylineno,yytext,COLON); return COLON; }
"+"                             { log_lex_info(*input_comp_files,yylineno,yytext,PLUS); return PLUS; }
"-"                             { log_lex_info(*input_comp_files,yylineno,yytext,MINUS); return MINUS; }
"*"                             { log_lex_info(*input_comp_files,yylineno,yytext,STAR); return STAR; }
"/"                             { log_lex_info(*input_comp_files,yylineno,yytext,SLASH); return SLASH; }
"%"                             { log_lex_info(*input_comp_files,yylineno,yytext,MOD); return MOD; }
"~"                             { log_lex_info(*input_comp_files,yylineno,yytext,TILDE); return TILDE; }
"|"                             { log_lex_info(*input_comp_files,yylineno,yytext,PIPE); return PIPE; }
"&"                             { log_lex_info(*input_comp_files,yylineno,yytext,AMP); return AMP; }
"!"                             { log_lex_info(*input_comp_files,yylineno,yytext,BANG); return BANG; }
"||"                            { log_lex_info(*input_comp_files,yylineno,yytext,DPIPE); return DPIPE; }
"&&"                            { log_lex_info(*input_comp_files,yylineno,yytext,DAMP); return DAMP; }
"="                             { log_lex_info(*input_comp_files,yylineno,yytext,ASSIGN); return ASSIGN; }
"+="                            { log_lex_info(*input_comp_files,yylineno,yytext,PLUSASSIGN); return PLUSASSIGN; }
"-="                            { log_lex_info(*input_comp_files,yylineno,yytext,MINUSASSIGN); return MINUSASSIGN; }
"*="                            { log_lex_info(*input_comp_files,yylineno,yytext,STARASSIGN); return STARASSIGN; }
"/="                            { log_lex_info(*input_comp_files,yylineno,yytext,SLASHASSIGN); return SLASHASSIGN; }
"++"                            { log_lex_info(*input_comp_files,yylineno,yytext,INCR); return INCR; }
"--"                            { log_lex_info(*input_comp_files,yylineno,yytext,DECR); return DECR; }
"=="                            { log_lex_info(*input_comp_files,yylineno,yytext,EQUALS); return EQUALS; }
"!="                            { log_lex_info(*input_comp_files,yylineno,yytext,NEQUAL); return NEQUAL; }
">"                             { log_lex_info(*input_comp_files,yylineno,yytext,GT); return GT; }
">="                            { log_lex_info(*input_comp_files,yylineno,yytext,GE); return GE; }
"<"                             { log_lex_info(*input_comp_files,yylineno,yytext,LT); return LT; }
"<="                            { log_lex_info(*input_comp_files,yylineno,yytext,LE); return LE; }
{ID}                            { log_lex_info(*input_comp_files,yylineno,yytext,IDENT); return IDENT; }
.                               { log_lex_err("Unexpected symbol",*input_comp_files,yylineno,yytext); }
<COM><<EOF>>                    { log_lex_err("Unclosed comment",*input_comp_files,openingCommentLine,NULL); return 1;}
<<EOF>>                         {if(*++input_comp_files) { yylineno = 1;yyin = fopen( *input_comp_files, "r" ); } else yyterminate();}
%%

void initfile(char* filename) {
    yyin = fopen( filename, "r" );
}