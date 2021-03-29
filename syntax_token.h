#ifndef SYNTAX_TOKEN_H
#define SYNTAX_TOKEN_H

#include "location.hh"
#include "mycc.tab.hpp"
#include <string>
#include <variant>
using token_type = enum yy::parser::token::yytokentype;
using token_variant = std::variant<int, float, char, std::string>;
using token_data_type = enum TOKEN_DATA_TYPE {
    INT,
    FLOAT,
    CHAR,
    STRING
};


class SyntaxToken {
private:
    std::string m_text;
    token_type m_token;
    std::string m_filename;
    int m_begin_line;
    int m_begin_column;
    int m_end_line;
    int m_end_column;
    token_variant m_semantic_value;
public:

    SyntaxToken(std::string text, 
                token_type token,
                std::string filename,
                int begin_line,
                int begin_column,
                int end_line,
                int end_column,
                token_variant semantic_value);
    
    ~SyntaxToken();


    /*
     * The text stored in this token.
     */
    std::string text() const;

    /*
     * The type of token this is, represented by the bison generated enum.
     */
    token_type token() const;

    /*
     * The filename of where this token is located.
     */
    std::string filename() const;

    /*
     * The token's starting location line-wise in the file.
     */
    int begin_line() const;

    /*
     * The token's starting location column-wise in the file.
     */
    int begin_column() const;

    /*
     * The token's ending location line-wise in the file.
     */
    int end_line() const;

    /*
     * The token's ending location column-wise in the file.
     */
    int end_column() const;

    /*
     * The semantic value of the token, stored as a variant type.
     */
    token_variant semantic_value() const;

    /*
     * Returns the int value stored in the variant
     * (if 'int' is the type currently stored).
     * 
     * If 'int' is not the curren type stored, 
     * this method will throw 'std::bad_variant_access'.
     */
    int get_int_value() const;

    /*
     * Returns the float value stored in the variant
     * (if 'float' is the type currently stored).
     * 
     * If 'float' is not the curren type stored, 
     * this method will throw 'std::bad_variant_access'.
     */
    float get_float_value() const;

    /*
     * Returns the char value stored in the variant
     * (if 'char' is the type currently stored).
     * 
     * If 'char' is not the curren type stored, 
     * this method will throw 'std::bad_variant_access'.
     */
    char get_char_value() const;

    /*
     * Returns the std::string value stored in the variant
     * (if 'std::string' is the type currently stored).
     * 
     * If 'std::string' is not the curren type stored, 
     * this method will throw 'std::bad_variant_access'.
     */
    std::string get_str_value() const;

    /*
     * Returns an enum representing the type that is
     * stored in the variant.
     */
    token_data_type get_type() const;

};

#endif