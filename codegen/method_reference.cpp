#include "method_reference.h"
#include "../binding/bound_function_definition_node.h"
#include "code_generator.h"

namespace CodeGen {

MethodReference::MethodReference(Symbols::FunctionSymbol *method,
                                 std::string owner, bool is_static,
                                 bool is_synthesized,
                                 Binding::BoundBlockStatementNode *body)
    : m_method(method), m_owner(owner), m_is_static(is_static),
      m_is_synthesized(is_synthesized), m_body(body) {}

MethodReference *
MethodReference::resolve_method(Symbols::FunctionSymbol *method,
                                std::string owner, bool is_static,
                                bool is_synthesized) {
  return new MethodReference(method, owner, is_static, is_synthesized);
}

MethodReference *MethodReference::resolve_and_define_method(
    Binding::BoundFunctionDefinitionNode *method, std::string owner,
    bool is_static, bool is_synthesized) {
  return new MethodReference(method->function_symbol(), owner, is_static,
                             is_synthesized, method->statements());
}

MethodReference *
MethodReference::define_method(Binding::BoundBlockStatementNode *body,
                               bool is_synthesized) {
  return new MethodReference(m_method, m_owner, m_is_static, is_synthesized,
                             body);
}

Symbols::FunctionSymbol *MethodReference::method() const { return m_method; }

std::string MethodReference::owner() const { return m_owner; }

bool MethodReference::is_static() const { return m_is_static; }

bool MethodReference::is_synthesized() const { return m_is_synthesized; }

bool MethodReference::is_defined() const { return m_body != nullptr; }

Binding::BoundBlockStatementNode *MethodReference::body() const {
  return m_body;
}

} // namespace CodeGen