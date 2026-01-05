#include "driver.h"
#include "mycc.tab.hpp"
#include "syntax/translation_unit_node.h"

Driver::Driver() : m_lexer(*this), m_parser(m_lexer, *this), m_location(0) {}

int Driver::parse() { return m_parser.parse(); }

std::vector<Syntax::TranslationUnitNode *> Driver::get_translation_units() {
  return m_translation_units;
}

std::vector<std::string> Driver::get_diagnostics() const {
  return m_diagnostics;
}

void Driver::init_new_input() { m_location.initialize(&curr_file); }

void Driver::switch_input_stream(std::string filename, std::istream *is) {
  // if (m_translation_units != nullptr) {

  // }
  curr_file = filename;
  m_lexer.switch_streams(is, NULL);
}

bool Driver::error_flag() const { return m_error_flag; }

yy::parser::symbol_type Driver::lex() { return m_lexer.get_next_token(); }

// void Driver::switch_input_file(std::string file) {
//     std::ifstream ifstrm(file);
//     std::istream* ist = &ifstrm;
//     switch_input_stream(ist);
//     //m_lexer.switch_streams(ist);
// }

yy::location Driver::location() const { return m_location; }

std::vector<LexemeDataNode> Driver::get_part_one_lexeme_list() const {
  return part_one_lexeme_list;
}

bool Driver::lex_and_check() {
  if (!m_reached_eof_or_error)
    lex();
  return m_reached_eof_or_error;
}

// void Driver::increaseLocation(unsigned int loc) {
//     m_location += loc;
//     std::cout << "increaseLocation(): " << loc << ", total = " << m_location
//     << std::endl;
// }

// unsigned int Driver::location() const {
//     return m_location;
// }
