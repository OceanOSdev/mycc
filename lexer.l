%{
#include <math.h>
#include <cstdlib>
#include <string>
#include "lexer.h"
#include "driver.h"
#include "logger.h"
#include "syntax_token.h"
#include "mycc.tab.hpp" // bison generated tokens


int openingCommentLine = 0;

// Original yyterminate() macro returns int. Since we're using Bison 3 variants
// as tokens, we must redefine it to change type from `int` to `Parser::semantic_type`
#define yyterminate() yy::parser::make_END(yy::location());

// This will track current scanner location.
// Action is called when length of the token is known.
#define YY_USER_ACTION  loc.step(); loc.columns(yyleng);
//m_driver.increaseLocation(yyleng);

SyntaxToken create_syntax_token(std::string text, token_type_t token, yy::location loc, std::string file);

SyntaxToken create_syntax_token_int(std::string text, token_type_t token, yy::location loc, int ival, std::string file);
SyntaxToken create_syntax_token_float(std::string text, token_type_t token, yy::location loc, float fval, std::string file);
SyntaxToken create_syntax_token_char(std::string text, token_type_t token, yy::location loc, char cval, std::string file);
SyntaxToken create_syntax_token_str(std::string text, token_type_t token, yy::location loc, std::string sval, std::string file);


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
"void"|"char"|"int"|"float"     { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, token_type_t::TYPE, loc, m_driver.curr_file); auto sym_info = yy::parser::make_TYPE(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::TYPE, std::string(yytext), false, "")); return sym_info; }
"const"                         { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_CONST(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::CONST, std::string(yytext), false, "")); return sym_info; } 
"struct"                        { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_STRUCT(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::STRUCT, std::string(yytext), false, "")); return sym_info; }
"for"                           { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_FOR(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::FOR, std::string(yytext), false, "")); return sym_info; }   
"while"                         { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_WHILE(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::WHILE, std::string(yytext), false, "")); return sym_info; } 
"do"                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_DO(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::DO, std::string(yytext), false, "")); return sym_info; }    
"if"                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_IF(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::IF, std::string(yytext), false, "")); return sym_info; }    
"else"                          { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_ELSE(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::ELSE, std::string(yytext), false, "")); return sym_info; }  
"break"                         { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_BREAK(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::BREAK, std::string(yytext), false, "")); return sym_info; } 
"continue"                      { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_CONTINUE(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::CONTINUE, std::string(yytext), false, "")); return sym_info; }
"return"                        { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_RETURN(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::RETURN, std::string(yytext), false, "")); return sym_info; }
{NUMBER}+                       { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_INTCONST(std::stoi(yytext), loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::INTCONST, std::string(yytext), false, "")); return sym_info; }
{NUMBER}+"."?{NUMBER}*          { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_REALCONST(std::stof(yytext), loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::REALCONST, std::string(yytext), false, "")); return sym_info; }
\"(\\.|[^"\\])*\"               { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_STRCONST(yytext, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::STRCONST, std::string(yytext), false, "")); return sym_info; }
\'([^\\]|\\[abnrt\'\\])\'       { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_CHARCONST(yytext[1], loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::CHARCONST, std::string(yytext), false, "")); return sym_info; }
"("                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_LPAR(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::LPAR, std::string(yytext), false, "")); return sym_info; }
")"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_RPAR(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::RPAR, std::string(yytext), false, "")); return sym_info; }
"["                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_LBRACKET(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::LBRACKET, std::string(yytext), false, "")); return sym_info; }
"]"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_RBRACKET(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::RBRACKET, std::string(yytext), false, "")); return sym_info; }
"{"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_LBRACE(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::LBRACE, std::string(yytext), false, "")); return sym_info; }
"}"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_RBRACE(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::RBRACE, std::string(yytext), false, "")); return sym_info; }
"."                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_DOT(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::DOT, std::string(yytext), false, "")); return sym_info; }
","                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_COMMA(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::COMMA, std::string(yytext), false, "")); return sym_info; }
";"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_SEMI(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::SEMI, std::string(yytext), false, "")); return sym_info; }
"?"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_QUEST(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::QUEST, std::string(yytext), false, "")); return sym_info; }
":"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_COLON(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::COLON, std::string(yytext), false, "")); return sym_info; }
"+"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_PLUS(Syntax::SyntaxTokenType::PLUS, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::PLUS, std::string(yytext), false, "")); return sym_info; }
"-"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_MINUS(Syntax::SyntaxTokenType::MINUS, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::MINUS, std::string(yytext), false, "")); return sym_info; }
"*"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_STAR(Syntax::SyntaxTokenType::STAR, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::STAR, std::string(yytext), false, "")); return sym_info; }
"/"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_SLASH(Syntax::SyntaxTokenType::SLASH, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::SLASH, std::string(yytext), false, "")); return sym_info; }
"%"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_MOD(Syntax::SyntaxTokenType::MOD, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::MOD, std::string(yytext), false, "")); return sym_info; }
"~"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_TILDE(Syntax::SyntaxTokenType::TILDE, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::TILDE, std::string(yytext), false, "")); return sym_info; }
"|"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_PIPE(Syntax::SyntaxTokenType::PIPE, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::PIPE, std::string(yytext), false, "")); return sym_info; }
"&"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_AMP(Syntax::SyntaxTokenType::AMP, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::AMP, std::string(yytext), false, "")); return sym_info; }
"!"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_BANG(Syntax::SyntaxTokenType::BANG, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::BANG, std::string(yytext), false, "")); return sym_info; }
"||"                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_DPIPE(Syntax::SyntaxTokenType::DPIPE, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::DPIPE, std::string(yytext), false, "")); return sym_info; }
"&&"                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_DAMP(Syntax::SyntaxTokenType::DAMP, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::DAMP, std::string(yytext), false, "")); return sym_info; }
"="                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_ASSIGN(Syntax::SyntaxTokenType::ASSIGN, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::ASSIGN, std::string(yytext), false, "")); return sym_info; }
"+="                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_PLUSASSIGN(Syntax::SyntaxTokenType::PLUSASSIGN, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::PLUSASSIGN, std::string(yytext), false, "")); return sym_info; }
"-="                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_MINUSASSIGN(Syntax::SyntaxTokenType::MINUSASSIGN, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::MINUSASSIGN, std::string(yytext), false, "")); return sym_info; }
"*="                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_STARASSIGN(Syntax::SyntaxTokenType::STARASSIGN, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::STARASSIGN, std::string(yytext), false, "")); return sym_info; }
"/="                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_SLASHASSIGN(Syntax::SyntaxTokenType::SLASHASSIGN, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::SLASHASSIGN, std::string(yytext), false, "")); return sym_info; }
"++"                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_INCR(Syntax::SyntaxTokenType::INCR, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::INCR, std::string(yytext), false, "")); return sym_info; }
"--"                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_DECR(Syntax::SyntaxTokenType::DECR, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::DECR, std::string(yytext), false, "")); return sym_info; }
"=="                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_EQUALS(Syntax::SyntaxTokenType::EQUALS, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::EQUALS, std::string(yytext), false, "")); return sym_info; }
"!="                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_NEQUAL(Syntax::SyntaxTokenType::NEQUAL, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::NEQUAL, std::string(yytext), false, "")); return sym_info; }
">"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_GT(Syntax::SyntaxTokenType::GT, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::GT, std::string(yytext), false, "")); return sym_info; }
">="                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_GE(Syntax::SyntaxTokenType::GE, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::GE, std::string(yytext), false, "")); return sym_info; }
"<"                             { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_LT(Syntax::SyntaxTokenType::LT, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::LT, std::string(yytext), false, "")); return sym_info; }
"<="                            { m_driver.curr_text = yytext; auto sym_info = yy::parser::make_LE(Syntax::SyntaxTokenType::LE, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::LE, std::string(yytext), false, "")); return sym_info; }
{ID}                            { m_driver.curr_text = yytext; auto token = create_syntax_token(yytext, token_type_t::IDENT, loc, m_driver.curr_file); auto sym_info = yy::parser::make_IDENT(token, loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::IDENT, std::string(yytext), false, "")); return sym_info; }
.                               { auto sym_info = yy::parser::make_END(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::END, std::string(yytext), true, "Unexpected symbol")); /*throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));*/ }
<COM><<EOF>>                    {  auto sym_info = yy::parser::make_END(loc); m_driver.part_one_lexeme_list.push_back(LexemeDataNode(sym_info, yy::parser::token::END, "", true, "Unclosed comment")); return yy::parser::make_END(yy::location());}
<<EOF>>                         { return yyterminate();}
%%

SyntaxToken create_syntax_token(std::string text, token_type_t token, yy::location loc, std::string file) {
  SyntaxToken s(text, token, file, loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, 0);
  //std::cout << "text: '" << s.text() << "' file: " << s.filename() << std::endl;
  return s;
}

SyntaxToken create_syntax_token_int(std::string text, token_type_t token, yy::location loc, int ival, std::string file) {
  SyntaxToken s(text, token, file, loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, ival);
  return s;
}

SyntaxToken create_syntax_token_float(std::string text, token_type_t token, yy::location loc, float fval, std::string file) {
  SyntaxToken s(text, token, file, loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, fval);
  return s;
}

SyntaxToken create_syntax_token_char(std::string text, token_type_t token, yy::location loc, char cval, std::string file) {
  SyntaxToken s(text, token, file, loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, cval);
  return s;
}

SyntaxToken create_syntax_token_str(std::string text, token_type_t token, yy::location loc, std::string sval, std::string file) {
  SyntaxToken s(text, token, file, loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, sval);
  return s;
}
