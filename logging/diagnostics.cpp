#include "diagnostics.h"
#include "../syntax/syntax_token.h"
#include <iostream>
#include <new>

namespace Logging {

Location::Location(int lineno, std::string file)
    : line_number(lineno), file_name(file) {}

Diagnostic::Diagnostic(Location loc, std::string msg)
    : m_location(loc), m_message(msg) {}

/*
 * The location in where this diagnostic was reported.
 */
Location Diagnostic::location() const { return m_location; }

/*
 * The message describing this diagnostic.
 */
std::string Diagnostic::message() const { return m_message; }

DiagnosticsList::DiagnosticsList() {}

std::vector<Diagnostic *> DiagnosticsList::diagnostics() const {
  return m_diagnostics;
}

Location DiagnosticsList::from_syntax_token(Syntax::SyntaxToken *token) {
  return Location(token->begin_line(), token->filename());
}

void DiagnosticsList::report(Location loc, std::string msg) {
  m_diagnostics.push_back(new Diagnostic(loc, msg));
}

void DiagnosticsList::report_redefinition_of_parameter(
    Syntax::SyntaxToken *token, std::string id) {
  Location loc = from_syntax_token(token);
  std::string message = "Redefinition of parameter '" + id + "'";
  report(loc, message);
}

void DiagnosticsList::report_expression_cannot_be_void(
    Syntax::SyntaxToken *token) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Statement requires expression of scalar type ('void' invalid)";
  report(loc, message);
}

void DiagnosticsList::report_no_definition_for_type_symbol(
    Syntax::SyntaxToken *token, std::string symbol_id) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Incomplete type '" + symbol_id + "' cannot be used here";
  report(loc, message);
}

void DiagnosticsList::report_conflicting_function_declarations(
    Syntax::SyntaxToken *token, std::string function_name) {
  Location loc = from_syntax_token(token);
  std::string message = "Conflicting types for function " + function_name;
  report(loc, message);
}

void DiagnosticsList::report_function_already_defined(
    Syntax::SyntaxToken *token, std::string function_name) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Function " + function_name + " has already been defined";
  report(loc, message);
}

void DiagnosticsList::report_invalid_return_type(Syntax::SyntaxToken *token,
                                                 std::string actual_type,
                                                 std::string expected_type) {
  Location loc = from_syntax_token(token);
  std::string message = "Invalid return of type '" + actual_type +
                        "' in method expecting '" + expected_type + "'";
  report(loc, message);
}

void DiagnosticsList::report_incompatible_conversion(
    Syntax::SyntaxToken *token, std::string actual_type,
    std::string expected_type) {
  Location loc = from_syntax_token(token);
  std::string message = "Incompatiple conversion assigning to '" +
                        expected_type + "' from '" + actual_type + "'";
  report(loc, message);
}

void DiagnosticsList::report_variable_already_declared(
    Syntax::SyntaxToken *token, std::string identifier, bool is_global_scope) {
  Location loc = from_syntax_token(token);
  std::string scope_id = is_global_scope ? "Global" : "Local";
  std::string message =
      scope_id + " variable " + identifier + " already declared";
  report(loc, message);
}

void DiagnosticsList::report_struct_already_defined(Syntax::SyntaxToken *token,
                                                    std::string identifier) {
  Location loc = from_syntax_token(token);
  std::string message = "struct " + identifier + " already defined.";
  report(loc, message);
}

void DiagnosticsList::report_variable_not_array_type(Syntax::SyntaxToken *token,
                                                     std::string identifier) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Subscripted value is not an array ('" + identifier + "')";
  report(loc, message);
}

void DiagnosticsList::report_undefined_variable(Syntax::SyntaxToken *token,
                                                std::string identifier) {
  Location loc = from_syntax_token(token);
  std::string message = "Use of undeclared identifier '" + identifier + "'";
  report(loc, message);
}

void DiagnosticsList::report_array_index_must_be_integer(
    Syntax::SyntaxToken *token, std::string identifier) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Array subscript is not an integer type ('" + identifier + "')";
  report(loc, message);
}

void DiagnosticsList::report_member_base_type_not_struct(
    Syntax::SyntaxToken *token, std::string identifier) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Member reference base type '" + identifier + "' is not a structure";
  report(loc, message);
}

void DiagnosticsList::report_no_member_in_type(Syntax::SyntaxToken *token,
                                               std::string member_identifier,
                                               std::string type_identifier) {
  Location loc = from_syntax_token(token);
  std::string message = "No member named '" + member_identifier + "' in '" +
                        type_identifier + "'";
  report(loc, message);
}

void DiagnosticsList::report_cannot_convert_explicitly(
    Syntax::SyntaxToken *token, std::string type_from, std::string type_to) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Cannot convert from type '" + type_from + "' to type '" + type_to + "'";
  report(loc, message);
}

void DiagnosticsList::report_invalid_binary_operator(Syntax::SyntaxToken *token,
                                                     std::string lhs_type,
                                                     std::string rhs_type) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Operator '" + syntax_token_type_to_string(token->token()) +
      "' undefined for types '" + lhs_type + "' and '" + rhs_type + "'";
  report(loc, message);
}

void DiagnosticsList::report_invalid_unary_operator(Syntax::SyntaxToken *token,
                                                    std::string type) {
  Location loc = from_syntax_token(token);
  std::string message = "Operator '" +
                        syntax_token_type_to_string(token->token()) +
                        "' undefined for type '" + type + "'";
  report(loc, message);
}

void DiagnosticsList::report_cannot_assign_to_constant(
    Syntax::SyntaxToken *token, std::string identifier) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Cannot assign to read-only variable '" + identifier + "'";
  report(loc, message);
}

void DiagnosticsList::report_cannot_assign_type_mismatch(
    Syntax::SyntaxToken *token, std::string type_from, std::string type_to) {
  Location loc = from_syntax_token(token);
  std::string message = "Type mismatch in assignment: expected '" + type_to +
                        "', got '" + type_from + "'";
  report(loc, message);
}

void DiagnosticsList::report_invalid_assignment_operator(
    Syntax::SyntaxToken *token, std::string lhs_type, std::string rhs_type) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Operator '" + syntax_token_type_to_string(token->token()) +
      "' undefined for types '" + lhs_type + "' and '" + rhs_type + "'";
  report(loc, message);
}

void DiagnosticsList::report_identifier_is_not_a_function(
    Syntax::SyntaxToken *token, std::string identifier) {
  Location loc = from_syntax_token(token);
  std::string message = "Called object '" + identifier + "' is not a function";
  report(loc, message);
}

void DiagnosticsList::report_function_not_declared(Syntax::SyntaxToken *token,
                                                   std::string identifier) {
  Location loc = from_syntax_token(token);
  std::string message = "Function '" + identifier + "' has not been declared";
  report(loc, message);
}

void DiagnosticsList::report_wrong_argument_count(Syntax::SyntaxToken *token,
                                                  std::string identifier,
                                                  bool too_few, int expected,
                                                  int actual) {
  Location loc = from_syntax_token(token);
  std::string modifier = too_few ? "few" : "many";
  std::string message = "Too " + modifier + " arguments to function '" +
                        identifier + "', expected " + std::to_string(expected) +
                        ", have " + std::to_string(actual);
  report(loc, message);
}

void DiagnosticsList::report_incompatible_argument(Syntax::SyntaxToken *token,
                                                   std::string identifier,
                                                   std::string actual_type,
                                                   std::string expected_type,
                                                   int argument_number) {
  Location loc = from_syntax_token(token);
  std::string message = "Argument " + std::to_string(argument_number) +
                        " in call to function '" + identifier + "': ";
  message +=
      " expected type '" + expected_type + "', but got '" + actual_type + "'";
  report(loc, message);
}

void DiagnosticsList::report_integer_type_required(Syntax::SyntaxToken *token,
                                                   std::string actual_type) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Used type '" + actual_type + "' where integer type is required";
  report(loc, message);
}

void DiagnosticsList::report_incompatible_operand_types(
    Syntax::SyntaxToken *token, std::string left_type, std::string right_type) {
  Location loc = from_syntax_token(token);
  std::string message = "Incompatible operand types ('" + left_type +
                        "' and '" + right_type + "')";
  report(loc, message);
}

void DiagnosticsList::report_invalid_condition_if(Syntax::SyntaxToken *token,
                                                  std::string type_id) {
  report_invalid_condition_type(token, "if statement", type_id);
}

void DiagnosticsList::report_invalid_condition_while(Syntax::SyntaxToken *token,
                                                     std::string type_id) {
  report_invalid_condition_type(token, "while loop", type_id);
}

void DiagnosticsList::report_invalid_condition_do_while(
    Syntax::SyntaxToken *token, std::string type_id) {
  report_invalid_condition_type(token, "do while loop", type_id);
}

void DiagnosticsList::report_invalid_condition_for(Syntax::SyntaxToken *token,
                                                   std::string type_id) {
  report_invalid_condition_type(token, "for loop", type_id);
}

void DiagnosticsList::report_invalid_condition_type(Syntax::SyntaxToken *token,
                                                    std::string statement_kind,
                                                    std::string type_id) {
  Location loc = from_syntax_token(token);
  std::string message =
      "Condition of " + statement_kind + " has invalid type: " + type_id;
  report(loc, message);
}

void DiagnosticsList::report_invalid_break_or_continue(
    Syntax::SyntaxToken *token, std::string text) {
  Location loc = from_syntax_token(token);
  std::string message = "Can only use '" + text + "' inside of a loop";
  report(loc, message);
}

void DiagnosticsList::report_cannot_increment_lval(Syntax::SyntaxToken *token,
                                                   std::string type) {
  Location loc = from_syntax_token(token);
  std::string message = "Cannot increment lvalue of type " + type;
  report(loc, message);
}

void DiagnosticsList::report_cannot_decrement_lval(Syntax::SyntaxToken *token,
                                                   std::string type) {
  Location loc = from_syntax_token(token);
  std::string message = "Cannot decrement lvalue of type " + type;
  report(loc, message);
}

void DiagnosticsList::report_all_paths_must_return(Syntax::SyntaxToken *token,
                                                   std::string function_id) {
  Location loc = from_syntax_token(token);
  std::string message =
      "All code paths in function '" + function_id + "' must return a value";
  report(loc, message);
}

std::string
DiagnosticsList::syntax_token_type_to_string(Syntax::token_type_t token_type) {
  switch (token_type) {
  case Syntax::token_type_t::PLUS:
    return "+";
    break;
  case Syntax::token_type_t::MINUS:
    return "-";
    break;
  case Syntax::token_type_t::STAR:
    return "*";
    break;
  case Syntax::token_type_t::SLASH:
    return "/";
    break;
  case Syntax::token_type_t::MOD:
    return "%";
    break;
  case Syntax::token_type_t::TILDE:
    return "~";
    break;
  case Syntax::token_type_t::PIPE:
    return "|";
    break;
  case Syntax::token_type_t::AMP:
    return "&";
    break;
  case Syntax::token_type_t::BANG:
    return "!";
    break;
  case Syntax::token_type_t::DPIPE:
    return "||";
    break;
  case Syntax::token_type_t::DAMP:
    return "&&";
    break;
  case Syntax::token_type_t::ASSIGN:
    return "=";
    break;
  case Syntax::token_type_t::PLUSASSIGN:
    return "+=";
    break;
  case Syntax::token_type_t::MINUSASSIGN:
    return "-=";
    break;
  case Syntax::token_type_t::STARASSIGN:
    return "*=";
    break;
  case Syntax::token_type_t::SLASHASSIGN:
    return "/=";
    break;
  case Syntax::token_type_t::INCR:
    return "++";
    break;
  case Syntax::token_type_t::DECR:
    return "--";
    break;
  case Syntax::token_type_t::EQUALS:
    return "==";
    break;
  case Syntax::token_type_t::NEQUAL:
    return "!=";
    break;
  case Syntax::token_type_t::GT:
    return ">";
    break;
  case Syntax::token_type_t::GE:
    return ">=";
    break;
  case Syntax::token_type_t::LT:
    return "<";
    break;
  case Syntax::token_type_t::LE:
    return "<=";
    break;
  default:
    return "ERROR";
  }
}

} // namespace Logging