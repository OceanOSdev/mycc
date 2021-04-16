#pragma once
#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <vector>
#include <string>

namespace Syntax {
    class SyntaxToken;
}

struct Location {
    const int line_number;
    const std::string file_name;

    Location(int lineno, std::string file);
};

class Diagnostic {
private:
    Location m_location;
    std::string m_message;
public:
    Diagnostic(Location loc, std::string msg);

    /*
     * The location in where this diagnostic was reported.
     */
    Location location() const;

    /*
     * The message describing this diagnostic.
     */
    std::string message() const;

};

class DiagnosticsList {
private:
    std::vector<Diagnostic*> m_diagnostics;

    void report(Location loc, std::string msg);
    Location from_syntax_token(Syntax::SyntaxToken* token);
public:
    DiagnosticsList();

    std::vector<Diagnostic*> diagnostics() const;
    void report_redefinition_of_parameter(Syntax::SyntaxToken* token, std::string id);
    void report_expression_cannot_be_void(Syntax::SyntaxToken* token);
    void report_no_definition_for_type_symbol(Syntax::SyntaxToken* token, std::string symbol_id);
    void report_conflicting_function_declarations(Syntax::SyntaxToken* token, std::string function_name);
    void report_function_already_defined(Syntax::SyntaxToken* token, std::string function_name);
    void report_invalid_return_type(Syntax::SyntaxToken* token, std::string actual_type, std::string expected_type);
    void report_incompatible_conversion(Syntax::SyntaxToken* token, std::string actual_type, std::string expected_type);
    void report_variable_already_declared(Syntax::SyntaxToken* token, std::string identifier, bool is_global_scope);
};

#endif