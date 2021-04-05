#ifndef DRIVER_H
#define DRIVER_H
#include "pch.h"
#include "lexer.h"
#include "mycc.tab.hpp"
#include "lexeme_data.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

// #define YY_DECL yy::parser::symbol_type yylex (Driver& drv)
// YY_DECL;

class Driver {
public:
    Driver();
    int parse();
    void switch_input_stream(std::string filename, std::istream *is);
    void init_new_input();
    yy::parser::symbol_type lex();
    std::vector<LexemeDataNode> get_part_one_lexeme_list() const;
    std::vector<Syntax::TranslationUnitNode*> get_translation_units();
    bool error_flag() const;
    friend class yy::parser;
    friend class Lexer;

private:
    // Used to get last Lexer location. Used in error messages.
    yy::location location() const;

    //unsigned int location() const;
    std::string curr_file;
    std::string curr_text; // <-- really disgusting I know
    Lexer m_lexer;
    bool m_error_flag = false;
    yy::parser m_parser;
    yy::location m_location;
    std::vector<LexemeDataNode> part_one_lexeme_list;
    std::vector<Syntax::TranslationUnitNode*> m_translation_units;
};
#endif