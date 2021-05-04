#include "j_asm_builder.h"

#include "../symbols/variable_symbol.h"
#include "../symbols/function_symbol.h"
#include "../symbols/parameter_symbol.h"
#include "../symbols/type_symbol.h"

#include "../binding/bound_variable_group_declaration_node.h"
#include "../binding/bound_literal_val_expression_node.h"
#include "../binding/bound_expression_node.h"
#include "../binding/bound_node_kind.h"


namespace JVMProcessor {

const std::set<std::string> JAsmBuilder::m_forbidden_words {
    "public",
    "private",
    "protected",
    "static",
    "final",
    "super",
    "synchronized",
    "open",
    "transitive",
    "volatile",
    "bridge",
    "static_phase",
    "transient",
    "varargs",
    "native",
    "interface",
    "abstract",
    "strict",
    "synthetic",
    "annotation",
    "enum",
    "module",
    "mandated",
    "strictfp" // <-- Krakatau assembly also disallows this one too, so we add it to our "forbidden" words.
};

std::string JAsmBuilder::lit_as_emittable_const(Binding::BoundLiteralValExpressionNode* lit) {
    auto type = lit->type();
    using namespace Symbols;
    if (TypeSymbol::are_types_equal(&TypeSymbol::Char, type))
        return std::to_string((int)lit->char_value());
    if (TypeSymbol::are_types_equal(&TypeSymbol::Int, type))
        return std::to_string(lit->int_value());
    if (TypeSymbol::are_types_equal(&TypeSymbol::Float, type))
        return (lit->float_value() > 0 ? "+" : "") + std::to_string(lit->float_value()) + "f";
    if (TypeSymbol::are_types_equal(&TypeSymbol::String, type))
        return "'" + lit->string_value() + "'";
    return "??"; // we know its a lit when we call this function so just return whatever.
}

std::string JAsmBuilder::str_as_emittable_identifier(std::string str) {
    if (m_forbidden_words.contains(str))
        return "\"" + str + "\"";
    return str;
}

std::string JAsmBuilder::emit_class_directive(std::string class_name) {
    std::string name = str_as_emittable_identifier(class_name);
    return ".class public " + name;
}

std::string JAsmBuilder::emit_super_directive() {
    return ".super java/lang/Object";
}

std::string JAsmBuilder::emit_field_directive(Binding::BoundVariableDeclarationNode* field_decl, bool is_static, bool& requires_clinit) {
    requires_clinit = false;
    auto field = field_decl->variable_symbol();
    bool is_const = field->is_const();
    std::string directive = ".field public ";
    std::string name = str_as_emittable_identifier(field->name());
    std::string type_sig = field->var_type()->jasm_str();

    if (is_static) directive += "static ";
    if (is_const) directive += "final ";
    directive += name + " ";
    directive += type_sig;

    if (field_decl->is_initialized()) {
        if (is_const && field_decl->initializer()->kind() == Binding::BoundNodeKind::LiteralExpression) {
            auto literal = dynamic_cast<Binding::BoundLiteralValExpressionNode*>(field_decl->initializer());
            if (Symbols::TypeSymbol::are_types_equal(literal->type(), &Symbols::TypeSymbol::String)) {
                requires_clinit = true;
                return directive;
            }
            return directive + " = " + lit_as_emittable_const(literal);

        }
        requires_clinit = true;
    }
    return directive;
}

std::string JAsmBuilder::emit_method_definition_header(Symbols::FunctionSymbol* method, bool is_static) {
    std::string directive = ".method public ";
    if (is_static) directive += "static ";
    directive += str_as_emittable_identifier(method->name()) + " : (";

    for (auto param : method->params()) {
        directive += param->type()->jasm_str();
    }

    directive += ")";
    directive += method->type()->jasm_str();
    return directive;
}

std::string JAsmBuilder::emit_method_code_directive(int max_stack_size, int local_count) {
    return ".code stack " + std::to_string(max_stack_size) + " locals " + std::to_string(local_count);
}

std::string JAsmBuilder::emit_directive_end(BlockDirectiveType directive_type) {
    switch (directive_type) {
        case BlockDirectiveType::ClassDirective:
            return ".end class";
        case BlockDirectiveType::MethodDirective:
            return ".end method";
        case BlockDirectiveType::CodeDirective:
            return ".end code";
        default:
            return ".end"; // we can't get here anyways, so just emit whatever.
    }
}


}