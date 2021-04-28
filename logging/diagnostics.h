#pragma once
#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <vector>
#include <string>

namespace Syntax {
    class SyntaxToken;
    enum btokentype : int;
}

namespace Logging {
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
    std::string syntax_token_type_to_string(Syntax::btokentype token_type);
    void report_invalid_condition_type(Syntax::SyntaxToken* token, std::string statement_kind, std::string type_id);
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
    void report_struct_already_defined(Syntax::SyntaxToken* token, std::string identifier);
    void report_variable_not_array_type(Syntax::SyntaxToken* token, std::string identifier);
    void report_undefined_variable(Syntax::SyntaxToken* token, std::string identifier);
    void report_array_index_must_be_integer(Syntax::SyntaxToken* token, std::string identifier);
    void report_member_base_type_not_struct(Syntax::SyntaxToken* token, std::string identifier);
    void report_no_member_in_type(Syntax::SyntaxToken* token, std::string member_identifier, std::string type_identifier);
    void report_cannot_convert_explicitly(Syntax::SyntaxToken* token, std::string type_from, std::string type_to);
    void report_invalid_binary_operator(Syntax::SyntaxToken* token, std::string lhs_type, std::string rhs_type);
    void report_invalid_unary_operator(Syntax::SyntaxToken* token, std::string type);
    void report_cannot_assign_to_constant(Syntax::SyntaxToken* token, std::string identifier);
    void report_cannot_assign_type_mismatch(Syntax::SyntaxToken* token, std::string type_from, std::string type_to);
    void report_invalid_assignment_operator(Syntax::SyntaxToken* token, std::string lhs_type, std::string rhs_type);
    void report_identifier_is_not_a_function(Syntax::SyntaxToken* token, std::string identifier);
    void report_function_not_declared(Syntax::SyntaxToken* token, std::string identifier);
    void report_wrong_argument_count(Syntax::SyntaxToken* token, std::string identifier, bool too_few, int expected, int actual);
    void report_incompatible_argument(Syntax::SyntaxToken* token, std::string identifier, std::string actual_type, std::string expected_type, int argument_number);
    void report_integer_type_required(Syntax::SyntaxToken* token, std::string actual_type);
    void report_incompatible_operand_types(Syntax::SyntaxToken* token, std::string left_type, std::string right_type);
    void report_invalid_condition_if(Syntax::SyntaxToken* token, std::string type_id);
    void report_invalid_condition_while(Syntax::SyntaxToken* token, std::string type_id);
    void report_invalid_condition_do_while(Syntax::SyntaxToken* token, std::string type_id);
    void report_invalid_condition_for(Syntax::SyntaxToken* token, std::string type_id);
    void report_invalid_break_or_continue(Syntax::SyntaxToken* token, std::string text);
    void report_cannot_increment_lval(Syntax::SyntaxToken* token, std::string type);
    void report_cannot_decrement_lval(Syntax::SyntaxToken* token, std::string type);
    void report_all_paths_must_return(Syntax::SyntaxToken* token, std::string function_id);
};
}
#endif