%{
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "log_utils.h"
#include "token_list.h"
#include "mycc.tab.h" // bison generated tokens
extern char*input_comp_file;
extern token_list_node_t* head;
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
"void"|"char"|"int"|"float"     { add_token(head,input_comp_file,yylineno,strdup(yytext),TYPE); return TYPE; }
"const"                         { add_token(head,input_comp_file,yylineno,strdup(yytext),CONST); return CONST; } 
"struct"                        { add_token(head,input_comp_file,yylineno,strdup(yytext),STRUCT); return STRUCT; }        
"for"                           { add_token(head,input_comp_file,yylineno,strdup(yytext),FOR); return FOR; }   
"while"                         { add_token(head,input_comp_file,yylineno,strdup(yytext),WHILE); return WHILE; } 
"do"                            { add_token(head,input_comp_file,yylineno,strdup(yytext),DO); return DO; }    
"if"                            { add_token(head,input_comp_file,yylineno,strdup(yytext),IF); return IF; }    
"else"                          { add_token(head,input_comp_file,yylineno,strdup(yytext),ELSE); return ELSE; }  
"break"                         { add_token(head,input_comp_file,yylineno,strdup(yytext),BREAK); return BREAK; } 
"continue"                      { add_token(head,input_comp_file,yylineno,strdup(yytext),CONTINUE); return CONTINUE; }      
"return"                        { add_token(head,input_comp_file,yylineno,strdup(yytext),RETURN); return RETURN; }
{NUMBER}+                       { add_token(head,input_comp_file,yylineno,strdup(yytext),INTCONST); return INTCONST; }
{NUMBER}+"."?{NUMBER}           { add_token(head,input_comp_file,yylineno,strdup(yytext),REALCONST); return REALCONST; }
\"(\\.|[^"\\])*\"               { add_token(head,input_comp_file,yylineno,strdup(yytext),STRCONST); return STRCONST; }
\'([^\\]|\\[abnrt\'\\])\'       { add_token(head,input_comp_file,yylineno,strdup(yytext),CHARCONST); return CHARCONST; }
"("                             { add_token(head,input_comp_file,yylineno,strdup(yytext),LPAR); return LPAR; }
")"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),RPAR); return RPAR; }
"["                             { add_token(head,input_comp_file,yylineno,strdup(yytext),LBRACKET); return LBRACKET; }
"]"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),RBRACKET); return RBRACKET; }
"{"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),LBRACE); return LBRACE; }
"}"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),RBRACE); return RBRACE; }
"."                             { add_token(head,input_comp_file,yylineno,strdup(yytext),DOT); return DOT; }
","                             { add_token(head,input_comp_file,yylineno,strdup(yytext),COMMA); return COMMA; }
";"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),SEMI); return SEMI; }
"?"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),QUEST); return QUEST; }
":"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),COLON); return COLON; }
"+"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),PLUS); return PLUS; }
"-"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),MINUS); return MINUS; }
"*"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),STAR); return STAR; }
"/"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),SLASH); return SLASH; }
"%"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),MOD); return MOD; }
"~"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),TILDE); return TILDE; }
"|"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),PIPE); return PIPE; }
"&"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),AMP); return AMP; }
"!"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),BANG); return BANG; }
"||"                            { add_token(head,input_comp_file,yylineno,strdup(yytext),DPIPE); return DPIPE; }
"&&"                            { add_token(head,input_comp_file,yylineno,strdup(yytext),DAMP); return DAMP; }
"="                             { add_token(head,input_comp_file,yylineno,strdup(yytext),ASSIGN); return ASSIGN; }
"+="                            { add_token(head,input_comp_file,yylineno,strdup(yytext),PLUSASSIGN); return PLUSASSIGN; }
"-="                            { add_token(head,input_comp_file,yylineno,strdup(yytext),MINUSASSIGN); return MINUSASSIGN; }
"*="                            { add_token(head,input_comp_file,yylineno,strdup(yytext),STARASSIGN); return STARASSIGN; }
"/="                            { add_token(head,input_comp_file,yylineno,strdup(yytext),SLASHASSIGN); return SLASHASSIGN; }
"++"                            { add_token(head,input_comp_file,yylineno,strdup(yytext),INCR); return INCR; }
"--"                            { add_token(head,input_comp_file,yylineno,strdup(yytext),DECR); return DECR; }
"=="                            { add_token(head,input_comp_file,yylineno,strdup(yytext),EQUALS); return EQUALS; }
"!="                            { add_token(head,input_comp_file,yylineno,strdup(yytext),NEQUAL); return NEQUAL; }
">"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),GT); return GT; }
">="                            { add_token(head,input_comp_file,yylineno,strdup(yytext),GE); return GE; }
"<"                             { add_token(head,input_comp_file,yylineno,strdup(yytext),LT); return LT; }
"<="                            { add_token(head,input_comp_file,yylineno,strdup(yytext),LE); return LE; }
{ID}                            { add_token(head,input_comp_file,yylineno,strdup(yytext),IDENT); return IDENT; }
.                               { add_token_err(head,"Unexpected symbol",input_comp_file,yylineno,strdup(yytext)); }
<COM><<EOF>>                    { add_token_err(head,"Unclosed comment",input_comp_file,openingCommentLine,NULL); return 1;}
<<EOF>>                         { yyterminate();}
%%