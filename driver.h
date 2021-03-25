#ifndef DRIVER_H
#define DRIVER_H
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
    friend class yy::parser;
    friend class Lexer;

private:
    // Used internally by Lexer YY_USER_ACTION to update location indicator
    //void increaseLocation(unsigned int loc);
    
    // Used to get last Lexer location. Used in error messages.
    yy::location location() const;
    //unsigned int location() const;
    std::string curr_file;
    Lexer m_lexer;
    yy::parser m_parser;
    yy::location m_location;
    std::vector<LexemeDataNode> part_one_lexeme_list;
};
#endif