#ifndef LEXER_H
#define LEXER_H

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif


#undef YY_DECL
#define YY_DECL yy::parser::symbol_type Lexer::get_next_token()

#include "mycc.tab.hpp" // this is needed for symbol_type

class Driver;

class Lexer : public yyFlexLexer {
    public:
        Lexer(Driver &driver) : m_driver(driver) {}
        virtual ~Lexer() {}
        virtual yy::parser::symbol_type get_next_token();
    
    private:
        Driver &m_driver;
};

#endif