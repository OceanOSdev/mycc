%{
#include <math.h>
#include <cstdlib>
#include <string>
#include "lexer.h"
#include "driver.h"
#include "logging/logger.h"
#include "syntax/syntax_token.h"
#include "mycc.tab.hpp" // bison generated tokens


int openingCommentLine = 0;

// Original yyterminate() macro returns int. Since we're using Bison 3 variants
// as tokens, we must redefine it to change type from `int` to `Parser::semantic_type`
#define yyterminate() yy::parser::make_END(yy::location());

// This will track current scanner location.
// Action is called when length of the token is known.
#define YY_USER_ACTION  loc.step(); loc.columns(yyleng);
//m_driver.increaseLocation(yyleng);

Syntax::SyntaxToken* create_syntax_token(std::string text, Syntax::token_type_t token, yy::location loc, std::string file);

Syntax::SyntaxToken* create_syntax_token_int(std::string text, Syntax::token_type_t token, yy::location loc, int ival, std::string file);
Syntax::SyntaxToken* create_syntax_token_float(std::string text, Syntax::token_type_t token, yy::location loc, float fval, std::string file);
Syntax::SyntaxToken* create_syntax_token_char(std::string text, Syntax::token_type_t token, yy::location loc, char cval, std::string file);
Syntax::SyntaxToken* create_syntax_token_str(std::string text, Syntax::token_type_t token, yy::location loc, std::string sval, std::string file);
std::string stripQuotes(std::string input);

%}


NUMBER [0-9]
ID     [a-zA-Z_][a-zA-Z0-9_]*
FNAME  [a-zA-Z0-9_]*\.[a-zA-Z0-9_]+

%x COM
%option yylineno
%option noyywrap
%option debug
%option nodefault
%option yyclass="Scanner"
%option c++

%%
%{
    // A handy shortcut to the location held by the driver.
  yy::location& loc = m_driver.m_location;
  
  // Code run each time yylex is called.
  //loc.step ();
%}

[ \t\r]*                        { /* ignore */ }
\n                              { loc.lines();}
"/*"                            { BEGIN(COM); openingCommentLine = lineno(); }
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
"void"|"char"|"int"|"float"     { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::TYPE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_TYPE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::TYPE, std::string(yytext), false, "")); return sym_info; }
"const"                         { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::CONST, loc, m_driver.curr_file); auto sym_info = yy::parser::make_CONST(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::CONST, std::string(yytext), false, "")); return sym_info; } 
"struct"                        { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::STRUCT, loc, m_driver.curr_file); auto sym_info = yy::parser::make_STRUCT(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::STRUCT, std::string(yytext), false, "")); return sym_info; }
"for"                           { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::FOR, loc, m_driver.curr_file); auto sym_info = yy::parser::make_FOR(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::FOR, std::string(yytext), false, "")); return sym_info; }   
"while"                         { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::WHILE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_WHILE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::WHILE, std::string(yytext), false, "")); return sym_info; } 
"do"                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::DO, loc, m_driver.curr_file); auto sym_info = yy::parser::make_DO(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::DO, std::string(yytext), false, "")); return sym_info; }    
"if"                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::IF, loc, m_driver.curr_file); auto sym_info = yy::parser::make_IF(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::IF, std::string(yytext), false, "")); return sym_info; }    
"else"                          { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::ELSE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_ELSE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::ELSE, std::string(yytext), false, "")); return sym_info; }  
"break"                         { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::BREAK, loc, m_driver.curr_file); auto sym_info = yy::parser::make_BREAK(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::BREAK, std::string(yytext), false, "")); return sym_info; } 
"continue"                      { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::CONTINUE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_CONTINUE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::CONTINUE, std::string(yytext), false, "")); return sym_info; }
"return"                        { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::RETURN, loc, m_driver.curr_file); auto sym_info = yy::parser::make_RETURN(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::RETURN, std::string(yytext), false, "")); return sym_info; }
{NUMBER}+                       { m_driver.curr_text = yytext; auto token = create_syntax_token_int(yytext, Syntax::token_type_t::INTCONST, loc, std::stoi(yytext), m_driver.curr_file); auto sym_info = yy::parser::make_INTCONST(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::INTCONST, std::string(yytext), false, "")); return sym_info; }
{NUMBER}+"."?{NUMBER}*          { m_driver.curr_text = yytext; auto token = create_syntax_token_float(yytext, Syntax::token_type_t::REALCONST, loc, std::stof(yytext), m_driver.curr_file); auto sym_info = yy::parser::make_REALCONST(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::REALCONST, std::string(yytext), false, "")); return sym_info; }
\"(\\.|[^"\\])*\"               { m_driver.curr_text = yytext; auto token = create_syntax_token_str(yytext, Syntax::token_type_t::STRCONST, loc, stripQuotes(yytext), m_driver.curr_file); auto sym_info = yy::parser::make_STRCONST(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::STRCONST, std::string(yytext), false, "")); return sym_info; }
\'([^\\]|\\[abnrt\'\\])\'       { m_driver.curr_text = yytext; auto token = create_syntax_token_char(yytext, Syntax::token_type_t::CHARCONST, loc, yytext[1], m_driver.curr_file); auto sym_info = yy::parser::make_CHARCONST(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::CHARCONST, std::string(yytext), false, "")); return sym_info; }
"("                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::LPAR, loc, m_driver.curr_file); auto sym_info = yy::parser::make_LPAR(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::LPAR, std::string(yytext), false, "")); return sym_info; }
")"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::RPAR, loc, m_driver.curr_file); auto sym_info = yy::parser::make_RPAR(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::RPAR, std::string(yytext), false, "")); return sym_info; }
"["                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::LBRACKET, loc, m_driver.curr_file); auto sym_info = yy::parser::make_LBRACKET(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::LBRACKET, std::string(yytext), false, "")); return sym_info; }
"]"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::RBRACKET, loc, m_driver.curr_file); auto sym_info = yy::parser::make_RBRACKET(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::RBRACKET, std::string(yytext), false, "")); return sym_info; }
"{"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::LBRACE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_LBRACE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::LBRACE, std::string(yytext), false, "")); return sym_info; }
"}"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::RBRACE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_RBRACE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::RBRACE, std::string(yytext), false, "")); return sym_info; }
"."                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::DOT, loc, m_driver.curr_file); auto sym_info = yy::parser::make_DOT(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::DOT, std::string(yytext), false, "")); return sym_info; }
","                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::COMMA, loc, m_driver.curr_file); auto sym_info = yy::parser::make_COMMA(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::COMMA, std::string(yytext), false, "")); return sym_info; }
";"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::SEMI, loc, m_driver.curr_file); auto sym_info = yy::parser::make_SEMI(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::SEMI, std::string(yytext), false, "")); return sym_info; }
"?"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::QUEST, loc, m_driver.curr_file); auto sym_info = yy::parser::make_QUEST(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::QUEST, std::string(yytext), false, "")); return sym_info; }
":"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::COLON, loc, m_driver.curr_file); auto sym_info = yy::parser::make_COLON(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::COLON, std::string(yytext), false, "")); return sym_info; }
"+"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::PLUS, loc, m_driver.curr_file); auto sym_info = yy::parser::make_PLUS(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::PLUS, std::string(yytext), false, "")); return sym_info; }
"-"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::MINUS, loc, m_driver.curr_file); auto sym_info = yy::parser::make_MINUS(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::MINUS, std::string(yytext), false, "")); return sym_info; }
"*"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::STAR, loc, m_driver.curr_file); auto sym_info = yy::parser::make_STAR(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::STAR, std::string(yytext), false, "")); return sym_info; }
"/"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::SLASH, loc, m_driver.curr_file); auto sym_info = yy::parser::make_SLASH(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::SLASH, std::string(yytext), false, "")); return sym_info; }
"%"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::MOD, loc, m_driver.curr_file); auto sym_info = yy::parser::make_MOD(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::MOD, std::string(yytext), false, "")); return sym_info; }
"~"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::TILDE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_TILDE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::TILDE, std::string(yytext), false, "")); return sym_info; }
"|"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::PIPE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_PIPE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::PIPE, std::string(yytext), false, "")); return sym_info; }
"&"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::AMP, loc, m_driver.curr_file); auto sym_info = yy::parser::make_AMP(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::AMP, std::string(yytext), false, "")); return sym_info; }
"!"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::BANG, loc, m_driver.curr_file); auto sym_info = yy::parser::make_BANG(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::BANG, std::string(yytext), false, "")); return sym_info; }
"||"                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::DPIPE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_DPIPE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::DPIPE, std::string(yytext), false, "")); return sym_info; }
"&&"                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::DAMP, loc, m_driver.curr_file); auto sym_info = yy::parser::make_DAMP(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::DAMP, std::string(yytext), false, "")); return sym_info; }
"="                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::ASSIGN, loc, m_driver.curr_file); auto sym_info = yy::parser::make_ASSIGN(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::ASSIGN, std::string(yytext), false, "")); return sym_info; }
"+="                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::PLUSASSIGN, loc, m_driver.curr_file); auto sym_info = yy::parser::make_PLUSASSIGN(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::PLUSASSIGN, std::string(yytext), false, "")); return sym_info; }
"-="                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::MINUSASSIGN, loc, m_driver.curr_file); auto sym_info = yy::parser::make_MINUSASSIGN(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::MINUSASSIGN, std::string(yytext), false, "")); return sym_info; }
"*="                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::STARASSIGN, loc, m_driver.curr_file); auto sym_info = yy::parser::make_STARASSIGN(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::STARASSIGN, std::string(yytext), false, "")); return sym_info; }
"/="                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::SLASHASSIGN, loc, m_driver.curr_file); auto sym_info = yy::parser::make_SLASHASSIGN(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::SLASHASSIGN, std::string(yytext), false, "")); return sym_info; }
"++"                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::INCR, loc, m_driver.curr_file); auto sym_info = yy::parser::make_INCR(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::INCR, std::string(yytext), false, "")); return sym_info; }
"--"                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::DECR, loc, m_driver.curr_file); auto sym_info = yy::parser::make_DECR(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::DECR, std::string(yytext), false, "")); return sym_info; }
"=="                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::EQUALS, loc, m_driver.curr_file); auto sym_info = yy::parser::make_EQUALS(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::EQUALS, std::string(yytext), false, "")); return sym_info; }
"!="                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::NEQUAL, loc, m_driver.curr_file); auto sym_info = yy::parser::make_NEQUAL(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::NEQUAL, std::string(yytext), false, "")); return sym_info; }
">"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::GT, loc, m_driver.curr_file); auto sym_info = yy::parser::make_GT(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::GT, std::string(yytext), false, "")); return sym_info; }
">="                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::GE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_GE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::GE, std::string(yytext), false, "")); return sym_info; }
"<"                             { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::LT, loc, m_driver.curr_file); auto sym_info = yy::parser::make_LT(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::LT, std::string(yytext), false, "")); return sym_info; }
"<="                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::LE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_LE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::LE, std::string(yytext), false, "")); return sym_info; }
{ID}                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, Syntax::token_type_t::IDENT, loc, m_driver.curr_file); auto sym_info = yy::parser::make_IDENT(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::IDENT, std::string(yytext), false, "")); return sym_info; }
.                               { auto sym_info = yy::parser::make_END(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::END, std::string(yytext), true, "Unexpected symbol")); /*throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));*/ }
<COM><<EOF>>                    {  auto sym_info = yy::parser::make_END(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::END, "", true, "Unclosed comment")); return yy::parser::make_END(yy::location());}
<<EOF>>                         { return yyterminate();}
%%

Syntax::SyntaxToken* create_syntax_token(std::string text, Syntax::token_type_t token, yy::location loc, std::string file) {
  return new Syntax::SyntaxToken(text, token, file, loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, 0);
}

Syntax::SyntaxToken* create_syntax_token_int(std::string text, Syntax::token_type_t token, yy::location loc, int ival, std::string file) {
  return new Syntax::SyntaxToken(text, token, file, loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, ival);
}

Syntax::SyntaxToken* create_syntax_token_float(std::string text, Syntax::token_type_t token, yy::location loc, float fval, std::string file) {
  return new Syntax::SyntaxToken(text, token, file, loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, fval);
}

Syntax::SyntaxToken* create_syntax_token_char(std::string text, Syntax::token_type_t token, yy::location loc, char cval, std::string file) {
  return new Syntax::SyntaxToken(text, token, file, loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, cval);
}

Syntax::SyntaxToken* create_syntax_token_str(std::string text, Syntax::token_type_t token, yy::location loc, std::string sval, std::string file) {
  return new Syntax::SyntaxToken(text, token, file, loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, sval);
}

std::string stripQuotes(std::string input) {
  return input.substr(1, input.length() - 2);
}
