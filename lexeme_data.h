#ifndef LEXEME_DATA_H
#define LEXEME_DATA_H
#include "mycc.tab.hpp"
#include <string>

typedef enum yy::parser::token::yytokentype yyparsetoken_t;

class LexemeDataNode {
    public:
    yy::parser::symbol_type symbol_info;
    yyparsetoken_t token;
    std::string text;
    bool is_error;
    std::string err_desc;
    int err_line_num;
    LexemeDataNode(yy::parser::symbol_type st, yyparsetoken_t t, std::string txt, bool err, std::string err_d, int er_l_n = -1) 
        : symbol_info(st), token(t), text(txt), is_error(err), err_desc(err_d), err_line_num(er_l_n) {}
};

#endif