#include "diagnostics.h"
#include "syntax/syntax_token.h"
#include <new>
#include <iostream>

Location::Location(int lineno, std::string file) :
    line_number(lineno), file_name(file) {}


Diagnostic::Diagnostic(Location loc, std::string msg) :
    m_location(loc), m_message(msg) {}

/*
 * The location in where this diagnostic was reported.
 */
Location Diagnostic::location() const {
    return m_location;
}

/*
 * The message describing this diagnostic.
 */
std::string Diagnostic::message() const {
    return m_message;
}

DiagnosticsList::DiagnosticsList() {}

std::vector<Diagnostic*> DiagnosticsList::diagnostics() const {
    return m_diagnostics;
}

Location DiagnosticsList::from_syntax_token(Syntax::SyntaxToken* token) {
    return Location(token->begin_line(), token->filename());
}

void DiagnosticsList::report(Location loc, std::string msg) {
    m_diagnostics.push_back(new Diagnostic(loc, msg));
}

void DiagnosticsList::report_redefinition_of_parameter(Syntax::SyntaxToken* token, std::string id) {
    Location loc = from_syntax_token(token);
    std::string message = "Redefinition of parameter '" + id + "'";
    report(loc,message);
}

void DiagnosticsList::report_expression_cannot_be_void(Syntax::SyntaxToken* token) {
    Location loc = from_syntax_token(token);
    std::string message = "Statement requires expression of scalar type ('void' invalid)";
    report(loc,message);
}

void DiagnosticsList::report_no_definition_for_type_symbol(Syntax::SyntaxToken* token, std::string symbol_id) {
    Location loc = from_syntax_token(token);
    std::string message = "No definition for '" + symbol_id + "'";
    report(loc,message);
}

void DiagnosticsList::report_conflicting_function_declarations(Syntax::SyntaxToken* token, std::string function_name) {
    Location loc = from_syntax_token(token);
    std::string message = "Conflicting types for function " + function_name;
    report(loc,message);
}