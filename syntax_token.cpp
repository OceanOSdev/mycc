#include "syntax_token.h"

SyntaxToken::SyntaxToken(std::string text, 
            token_type token,
            std::string filename,
            int begin_line,
            int begin_column,
            int end_line,
            int end_column,
            token_variant semantic_value) :
    m_text(text),
    m_token(token),
    m_filename(filename),
    m_begin_line(begin_line),
    m_begin_column(begin_column),
    m_end_line(end_line),
    m_end_column(end_column),
    m_semantic_value(semantic_value) { }

SyntaxToken::~SyntaxToken() {
}


/*
 * The text stored in this token.
 */
std::string SyntaxToken::text() const {
    return m_text;
}

/*
 * The type of token this is, represented by the bison generated enum.
 */
token_type SyntaxToken::token() const {
    return m_token;
}

/*
 * The filename of where this token is located.
 */
std::string SyntaxToken::filename() const {
    return m_filename;
}

/*
 * The token's starting location line-wise in the file.
 */
int SyntaxToken::begin_line() const {
    return m_begin_line;
}

/*
 * The token's starting location column-wise in the file.
 */
int SyntaxToken::begin_column() const {
    return m_begin_column;
}

/*
 * The token's ending location line-wise in the file.
 */
int SyntaxToken::end_line() const {
    return m_end_line;
}

/*
 * The token's ending location column-wise in the file.
 */
int SyntaxToken::end_column() const {
    return m_end_column;
}

/*
 * The semantic value of the token, stored as a variant type.
 */
token_variant SyntaxToken::semantic_value() const {
    return m_semantic_value;
}

/*
 * Returns the int value stored in the variant
 * (if 'int' is the type currently stored).
 * 
 * If 'int' is not the curren type stored, 
 * this method will throw 'std::bad_variant_access'.
 */
int SyntaxToken::get_int_value() const {
    return std::get<int>(m_semantic_value);
}

/*
 * Returns the float value stored in the variant
 * (if 'float' is the type currently stored).
 * 
 * If 'float' is not the curren type stored, 
 * this method will throw 'std::bad_variant_access'.
 */
float SyntaxToken::get_float_value() const {
    return std::get<float>(m_semantic_value);
}

/*
 * Returns the char value stored in the variant
 * (if 'char' is the type currently stored).
 * 
 * If 'char' is not the curren type stored, 
 * this method will throw 'std::bad_variant_access'.
 */
char SyntaxToken::get_char_value() const {
    return std::get<char>(m_semantic_value);
}

/*
 * Returns the std::string value stored in the variant
 * (if 'std::string' is the type currently stored).
 * 
 * If 'std::string' is not the curren type stored, 
 * this method will throw 'std::bad_variant_access'.
 */
std::string SyntaxToken::get_str_value() const {
    return std::get<std::string>(m_semantic_value);
}

/*
 * Returns an enum representing the type that is
 * stored in the variant.
 */
token_data_type SyntaxToken::get_type() const {
    std::visit([](std::variant<int, float, char, std::string>&& arg) {
        if (std::holds_alternative<int>(arg)) {
            return token_data_type::INT;
        } else if (std::holds_alternative<char>(arg)) {
            return token_data_type::CHAR;
        } else if (std::holds_alternative<float>(arg)) {
            return token_data_type::FLOAT;
        } else if (std::holds_alternative<std::string>(arg)) {
            return token_data_type::STRING;
        }
    }, m_semantic_value);
}