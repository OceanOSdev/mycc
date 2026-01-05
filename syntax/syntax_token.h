#pragma once
#ifndef SYNTAX_TOKEN_H
#define SYNTAX_TOKEN_H
#include <string>
#include <variant>

namespace Syntax {
// Hack to get around bison making it super
// inconvinient to get token info outside of .l and .ypp files
enum btokentype : int {
  END = 0,
  IDENT = 258,
  TYPE = 259,
  CONST = 260,
  STRUCT = 261,
  INTCONST = 262,
  REALCONST = 263,
  STRCONST = 264,
  CHARCONST = 265,
  FOR = 266,
  WHILE = 267,
  DO = 268,
  IF = 269,
  ELSE = 270,
  BREAK = 271,
  CONTINUE = 272,
  RETURN = 273,
  LPAR = 274,
  RPAR = 275,
  LBRACKET = 276,
  RBRACKET = 277,
  LBRACE = 278,
  RBRACE = 279,
  DOT = 280,
  COMMA = 281,
  SEMI = 282,
  QUEST = 283,
  COLON = 284,
  PLUS = 285,
  MINUS = 286,
  STAR = 287,
  SLASH = 288,
  MOD = 289,
  TILDE = 290,
  PIPE = 291,
  AMP = 292,
  BANG = 293,
  DPIPE = 294,
  DAMP = 295,
  ASSIGN = 296,
  PLUSASSIGN = 297,
  MINUSASSIGN = 298,
  STARASSIGN = 299,
  SLASHASSIGN = 300,
  INCR = 301,
  DECR = 302,
  EQUALS = 303,
  NEQUAL = 304,
  GT = 305,
  GE = 306,
  LT = 307,
  LE = 308,
  WITHOUT_ELSE = 309,
  UMINUS = 310
};

enum TOKEN_DATA_TYPE : int { INT, FLOAT, CHAR, STRING };

using token_type_t = btokentype;
using token_variant = std::variant<int, float, char, std::string>;
using token_data_type = TOKEN_DATA_TYPE;

class SyntaxToken {
private:
  std::string m_text;
  token_type_t m_token;
  std::string m_filename;
  int m_begin_line;
  int m_begin_column;
  int m_end_line;
  int m_end_column;
  token_variant m_semantic_value;

public:
  SyntaxToken();
  SyntaxToken(std::string text, token_type_t token, std::string filename,
              int begin_line, int begin_column, int end_line, int end_column,
              token_variant semantic_value);

  SyntaxToken(std::string text, token_type_t token, std::string filename,
              int begin_line, int begin_column, int end_line, int end_column,
              int i_val);

  SyntaxToken(std::string text, token_type_t token, std::string filename,
              int begin_line, int begin_column, int end_line, int end_column,
              float f_val);

  SyntaxToken(std::string text, token_type_t token, std::string filename,
              int begin_line, int begin_column, int end_line, int end_column,
              char char_val);

  SyntaxToken(std::string text, token_type_t token, std::string filename,
              int begin_line, int begin_column, int end_line, int end_column,
              std::string str_val);

  ~SyntaxToken();

  /*
   * The text stored in this token.
   */
  std::string text() const;

  /*
   * The type of token this is, represented by the bison generated enum.
   */
  token_type_t token() const;

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

} // namespace Syntax
#endif