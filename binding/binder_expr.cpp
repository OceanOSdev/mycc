/* LIB INCLUDES */
#include <stdexcept>

/* HEADER INCLUDE */
#include "binder.h"

/* UTILITY INCLUDES */
#include "../logging/diagnostics.h"

/* SYMBOL INCLUDES */
#include "../symbols/function_symbol.h"
#include "../symbols/parameter_symbol.h"
#include "../symbols/struct_symbol.h"
#include "../symbols/type_symbol.h"
#include "../symbols/variable_symbol.h"

/* BOUND TREE NODE INCLUDES */
#include "bound_assignment_expression_node.h"
#include "bound_binary_expression_node.h"
#include "bound_call_expression_node.h"
#include "bound_cast_expression_node.h"
#include "bound_error_expression_node.h"
#include "bound_increment_expression_node.h"
#include "bound_index_expression_node.h"
#include "bound_literal_val_expression_node.h"
#include "bound_member_access_expression_node.h"
#include "bound_scope.h"
#include "bound_ternary_expression_node.h"
#include "bound_unary_expression_node.h"
#include "bound_variable_reference_expression_node.h"

/* SYNTAX TREE NODE INCLUDES */
#include "../syntax/assignment_expression_node.h"
#include "../syntax/binary_expression_node.h"
#include "../syntax/call_expression_node.h"
#include "../syntax/cast_expression_node.h"
#include "../syntax/decrement_expression_node.h"
#include "../syntax/expression_node.h"
#include "../syntax/increment_expression_node.h"
#include "../syntax/index_expression_node.h"
#include "../syntax/literal_val_expression_node.h"
#include "../syntax/member_expression_node.h"
#include "../syntax/name_expression_node.h"
#include "../syntax/struct_declaration_node.h"
#include "../syntax/syntax_kind.h"
#include "../syntax/ternary_expression_node.h"
#include "../syntax/unary_expression_node.h"

namespace Binding {

BoundExpressionNode *Binder::bind_error_expression() {
  m_err_flag = true;
  return new BoundErrorExpressionNode();
}

BoundExpressionNode *Binder::bind_expression(Syntax::ExpressionNode *expression,
                                             bool canBeVoid) {
  if (m_err_flag)
    return new BoundErrorExpressionNode();
  auto result = bind_expression_internal(expression);
  if (Symbols::TypeSymbol::are_types_equivalent(result->type(),
                                                &Symbols::TypeSymbol::Error))
    return new BoundErrorExpressionNode(); // m_err_flag should already be set,
                                           // in case of bugs though, take a
                                           // gander here.
  if (!canBeVoid && Symbols::TypeSymbol::are_types_equivalent(
                        result->type(), &Symbols::TypeSymbol::Void)) {
    m_diagnostics->report_expression_cannot_be_void(expression->token());
    return bind_error_expression();
  }
  return result;
}

BoundExpressionNode *
Binder::bind_expression_internal(Syntax::ExpressionNode *expression) {
  auto syntaxKind = expression->kind();
  switch (syntaxKind) {
  case Syntax::SyntaxKind::AssignmentExpression:
    return bind_assignment_expression(
        dynamic_cast<Syntax::AssignmentExpressionNode *>(expression));
  case Syntax::SyntaxKind::BinaryExpression:
    return bind_binary_expression(
        dynamic_cast<Syntax::BinaryExpressionNode *>(expression));
  case Syntax::SyntaxKind::CallExpression:
    return bind_call_expression(
        dynamic_cast<Syntax::CallExpressionNode *>(expression));
  case Syntax::SyntaxKind::CastExpression:
    return bind_cast_expression(
        dynamic_cast<Syntax::CastExpressionNode *>(expression));
  case Syntax::SyntaxKind::DecrementExpression:
    return bind_decrement_expression(
        dynamic_cast<Syntax::DecrementExpressionNode *>(expression));
  case Syntax::SyntaxKind::IncrementExpression:
    return bind_increment_expression(
        dynamic_cast<Syntax::IncrementExpressionNode *>(expression));
  case Syntax::SyntaxKind::IndexExpression:
    return bind_index_expression(
        dynamic_cast<Syntax::IndexExpressionNode *>(expression));
  case Syntax::SyntaxKind::LiteralValExpression:
    return bind_literal_val_expression(
        dynamic_cast<Syntax::LiteralValExpressionNode *>(expression));
  case Syntax::SyntaxKind::MemberExpression:
    return bind_member_expression(
        dynamic_cast<Syntax::MemberExpressionNode *>(expression));
  case Syntax::SyntaxKind::NameExpression:
    return bind_name_expression(
        dynamic_cast<Syntax::NameExpressionNode *>(expression));
  case Syntax::SyntaxKind::TernaryExpression:
    return bind_ternary_expression(
        dynamic_cast<Syntax::TernaryExpressionNode *>(expression));
  case Syntax::SyntaxKind::UnaryExpression:
    return bind_unary_expression(
        dynamic_cast<Syntax::UnaryExpressionNode *>(expression));
  default:
    throw std::runtime_error("Unexpected syntax while binding expression.");
  }

  return bind_error_expression();
}

BoundExpressionNode *Binder::bind_assignment_expression(
    Syntax::AssignmentExpressionNode *assignment_expression) {
  auto bound_expression = bind_expression(assignment_expression->expression());
  auto bound_identifier_expression =
      bind_expression(assignment_expression->identifier_expression());

  if (Symbols::TypeSymbol::is_error_type(bound_identifier_expression->type()))
    return bind_error_expression();

  auto lvar_ref = dynamic_cast<BoundVariableReferenceExpressionNode *>(
      bound_identifier_expression);

  // if the assignee is not an lvalue type, return an error type
  if (lvar_ref == nullptr)
    return bind_error_expression();

  Symbols::VariableSymbol *variable = lvar_ref->variable_reference();

  if (variable->is_const()) {
    m_diagnostics->report_cannot_assign_to_constant(
        assignment_expression->token(), variable->name());
    return bind_error_expression();
  }

  if (!Symbols::TypeSymbol::are_types_equivalent(bound_expression->type(),
                                                 lvar_ref->type())) {
    m_diagnostics->report_cannot_assign_type_mismatch(
        assignment_expression->token(), bound_expression->type()->str(),
        variable->var_type()->str());
    return bind_error_expression();
  }

  if (bound_expression->type()->attributes().is_array &&
      lvar_ref->type()->attributes().is_array) {
    m_diagnostics->report_invalid_assignment_operator(
        assignment_expression->token(), lvar_ref->type()->str(),
        bound_expression->type()->str());
    return bind_error_expression();
  }

  BoundAssignmentOpKind assignment_op_kind;
  bool rewrite = true;
  Syntax::token_type_t rewrite_op_kind;
  switch (assignment_expression->assignment_type()) {
  case Syntax::token_type_t::ASSIGN:
    assignment_op_kind = BoundAssignmentOpKind::ASSIGN;
    rewrite = false;
    break;
  case Syntax::token_type_t::PLUSASSIGN:
    assignment_op_kind = BoundAssignmentOpKind::PLUS_ASSIGN;
    rewrite_op_kind = Syntax::token_type_t::PLUS;
    break;
  case Syntax::token_type_t::MINUSASSIGN:
    assignment_op_kind = BoundAssignmentOpKind::MINUS_ASSIGN;
    rewrite_op_kind = Syntax::token_type_t::MINUS;
    break;
  case Syntax::token_type_t::STARASSIGN:
    assignment_op_kind = BoundAssignmentOpKind::STAR_ASSIGN;
    rewrite_op_kind = Syntax::token_type_t::STAR;
    break;
  case Syntax::token_type_t::SLASHASSIGN:
    assignment_op_kind = BoundAssignmentOpKind::SLASH_ASSIGN;
    rewrite_op_kind = Syntax::token_type_t::SLASH;
    break;
  default:
    return bind_error_expression();
  }

  if (rewrite) {
    auto bound_bin_op = BoundBinaryOperatorNode::Bind(
        rewrite_op_kind, lvar_ref->type(), bound_expression->type());
    if (bound_bin_op == nullptr) {
      m_diagnostics->report_invalid_assignment_operator(
          assignment_expression->token(), variable->var_type()->str(),
          bound_expression->type()->str());
      return bind_error_expression();
    }
  }

  return new BoundAssignmentExpressionNode(assignment_op_kind, lvar_ref,
                                           bound_expression);
}

BoundExpressionNode *Binder::bind_binary_expression(
    Syntax::BinaryExpressionNode *binary_expression) {
  auto bound_left = bind_expression(binary_expression->left_expression());
  auto bound_right = bind_expression(binary_expression->right_expression());

  if (Symbols::TypeSymbol::is_error_type(bound_left->type()) ||
      Symbols::TypeSymbol::is_error_type(bound_right->type()))
    return bind_error_expression();

  auto bound_operator =
      BoundBinaryOperatorNode::Bind(binary_expression->syntax_token_type(),
                                    bound_left->type(), bound_right->type());

  if (bound_operator == nullptr) {
    m_diagnostics->report_invalid_binary_operator(binary_expression->token(),
                                                  bound_left->type()->str(),
                                                  bound_right->type()->str());
    return bind_error_expression();
  }

  return new BoundBinaryExpressionNode(bound_operator, bound_left, bound_right);
}

BoundExpressionNode *
Binder::bind_call_expression(Syntax::CallExpressionNode *call_expression) {
  Symbols::FunctionSymbol *symbol;
  std::string identifier = call_expression->name();
  auto exists = m_scope->try_look_up_function(identifier, symbol);
  if (!exists) {
    m_diagnostics->report_function_not_declared(call_expression->token(),
                                                identifier);
    return bind_error_expression();
  }
  // if (symbol->kind() != Symbols::SymbolKind::FUNCTION) {
  //     m_diagnostics->report_identifier_is_not_a_function(call_expression->token(),
  //     identifier); return bind_error_expression();
  // }

  auto function = dynamic_cast<Symbols::FunctionSymbol *>(symbol);

  if (function->params().size() != call_expression->expressions().size()) {
    bool too_few =
        function->params().size() > call_expression->expressions().size();
    m_diagnostics->report_wrong_argument_count(
        call_expression->token(), identifier, too_few,
        function->params().size(), call_expression->expressions().size());
    return bind_error_expression();
  }

  std::vector<BoundExpressionNode *> bound_arguments;
  for (std::vector<Symbols::ParameterSymbol *>::size_type arg_idx = 0;
       arg_idx < function->params().size(); arg_idx++) {
    auto expected_type = function->params()[arg_idx]->type();
    auto bound_expression =
        bind_expression(call_expression->expressions()[arg_idx]);
    auto actual_type = bound_expression->type();

    if (!Symbols::TypeSymbol::are_types_equivalent(actual_type,
                                                   expected_type)) {
      auto token = call_expression->expressions()[arg_idx]->token();
      m_diagnostics->report_incompatible_argument(
          token, identifier, actual_type->str(), expected_type->str(),
          int(arg_idx + 1));
      return bind_error_expression();
    }

    bound_arguments.push_back(bound_expression);
  }

  return new BoundCallExpressionNode(function, bound_arguments);
}

BoundExpressionNode *
Binder::bind_cast_expression(Syntax::CastExpressionNode *cast_expression) {
  const Symbols::TypeSymbol *type_symbol = nullptr;
  if (!m_scope->try_look_up_type(cast_expression->type(), type_symbol)) {
    m_diagnostics->report_no_definition_for_type_symbol(
        cast_expression->token(), cast_expression->type());
    return bind_error_expression();
  }

  auto bound_expression = bind_expression(cast_expression->expression());
  if (m_err_flag)
    return new BoundErrorExpressionNode();

  // can_be_explicitly_casted implicitly requires that both types are numeric
  // types.
  if (!Symbols::TypeSymbol::can_be_explicitly_casted(bound_expression->type(),
                                                     type_symbol)) {
    m_diagnostics->report_cannot_convert_explicitly(
        cast_expression->token(), bound_expression->type()->str(),
        type_symbol->str());
    return bind_error_expression();
  }

  if (bound_expression->type()->attributes().is_const) {
    type_symbol = type_symbol->as_const_type();
  }

  return new BoundCastExpressionNode(type_symbol, bound_expression);
}

BoundExpressionNode *Binder::bind_decrement_expression(
    Syntax::DecrementExpressionNode *decrement_expression) {
  auto notation = decrement_expression->is_post() ? IDNotation::POSTFIX
                                                  : IDNotation::PREFIX;
  auto bound_expr =
      bind_expression(decrement_expression->identifier_expression());
  if (bound_expr->type()->attributes().is_const ||
      bound_expr->type()->attributes().is_array) {
    m_diagnostics->report_cannot_decrement_lval(decrement_expression->token(),
                                                bound_expr->type()->str());
    return bind_error_expression();
  }
  return new BoundDecrementExpressionNode(notation, bound_expr);
}

BoundExpressionNode *Binder::bind_increment_expression(
    Syntax::IncrementExpressionNode *increment_expression) {
  auto notation = increment_expression->is_post() ? IDNotation::POSTFIX
                                                  : IDNotation::PREFIX;
  auto bound_expr =
      bind_expression(increment_expression->identifier_expression());
  if (bound_expr->type()->attributes().is_const ||
      bound_expr->type()->attributes().is_array) {
    m_diagnostics->report_cannot_increment_lval(increment_expression->token(),
                                                bound_expr->type()->str());
    return bind_error_expression();
  }
  return new BoundIncrementExpressionNode(notation, bound_expr);
}

BoundExpressionNode *
Binder::bind_index_expression(Syntax::IndexExpressionNode *index_expression) {
  Symbols::VariableSymbol *variable = nullptr;
  if (m_struct_scope.empty() || m_struct_scope.top() == nullptr) {
    if (!m_scope->try_look_up_variable(index_expression->name(), variable)) {
      m_diagnostics->report_undefined_variable(index_expression->token(),
                                               index_expression->name());
      return bind_error_expression();
    }
  } else {
    for (auto member : m_struct_scope.top()->members()) {
      if (member->name() == index_expression->name()) {
        variable = member;
        break;
      }
    }

    if (variable == nullptr) {
      m_diagnostics->report_no_member_in_type(index_expression->token(),
                                              index_expression->name(),
                                              m_struct_scope.top()->name());
      return bind_error_expression();
    }
  }

  if (!variable->is_array()) {
    m_diagnostics->report_variable_not_array_type(index_expression->token(),
                                                  index_expression->name());
    return bind_error_expression();
  }

  m_struct_scope.push(nullptr); // probably a terrible way to do this
  auto bound_index_value_expression =
      bind_expression(index_expression->expression());
  m_struct_scope.pop();
  if (!bound_index_value_expression->type()->attributes().is_integer_type) {
    m_diagnostics->report_array_index_must_be_integer(index_expression->token(),
                                                      variable->name());
    return bind_error_expression();
  }

  return new BoundIndexExpressionNode(variable, bound_index_value_expression);
}

BoundExpressionNode *Binder::bind_literal_val_expression(
    Syntax::LiteralValExpressionNode *literal_expression) {
  auto literal_kind = literal_expression->value_type();
  switch (literal_kind) {
  case Syntax::TOKEN_DATA_TYPE::CHAR:
    return new BoundLiteralValExpressionNode(literal_expression->char_value());
  case Syntax::TOKEN_DATA_TYPE::INT:
    return new BoundLiteralValExpressionNode(literal_expression->int_value());
  case Syntax::TOKEN_DATA_TYPE::FLOAT:
    return new BoundLiteralValExpressionNode(literal_expression->float_value());
  case Syntax::TOKEN_DATA_TYPE::STRING:
    return new BoundLiteralValExpressionNode(
        literal_expression->string_value());
  }
  return new BoundErrorExpressionNode();
}

BoundExpressionNode *Binder::bind_member_expression(
    Syntax::MemberExpressionNode *member_expression) {
  auto encapsulating_variable_expression =
      bind_expression(member_expression->encapsulator());
  if (m_err_flag)
    return new BoundErrorExpressionNode();
  auto encapsulating_var_ref =
      dynamic_cast<BoundVariableReferenceExpressionNode *>(
          encapsulating_variable_expression);
  auto encap_type = encapsulating_var_ref->type();
  // auto encapsulating_var_ref_type = encapsulating_var_ref->type();

  // check if encapsulating type is a struct
  // const Symbols::TypeSymbol* encap_type;
  // m_scope->try_look_up_type(encapsulating_var_ref_type->name(), encap_type);

  // don't have to check if type exists since that would be caught when binding
  // the name expression of of 'encapsulating_variable_expression'
  if (!encap_type->attributes().is_struct ||
      encap_type->attributes().is_array) {
    m_diagnostics->report_member_base_type_not_struct(
        member_expression->token(), encap_type->str());
    return bind_error_expression();
  }

  Symbols::StructSymbol *struct_symbol;
  m_scope->try_look_up_struct(encap_type->name(), struct_symbol);

  m_struct_scope.push(struct_symbol);
  auto bound_expression = bind_expression(member_expression->member());
  m_struct_scope.pop();
  if (m_err_flag)
    return new BoundErrorExpressionNode();

  auto bound_var_ref_expr =
      dynamic_cast<BoundVariableReferenceExpressionNode *>(bound_expression);

  return new BoundMemberAccessExpressionNode(bound_var_ref_expr,
                                             encapsulating_var_ref);
}

BoundExpressionNode *
Binder::bind_name_expression(Syntax::NameExpressionNode *name_expression) {
  Symbols::VariableSymbol *variable = nullptr;
  if (m_struct_scope.empty() || m_struct_scope.top() == nullptr) {
    if (!m_scope->try_look_up_variable(name_expression->name(), variable)) {
      m_diagnostics->report_undefined_variable(name_expression->token(),
                                               name_expression->name());
      return bind_error_expression();
    }
  } else {
    for (auto member : m_struct_scope.top()->members()) {
      if (member->name() == name_expression->name()) {
        variable = member;
        break;
      }
    }

    if (variable == nullptr) {
      m_diagnostics->report_no_member_in_type(name_expression->token(),
                                              name_expression->name(),
                                              m_struct_scope.top()->name());
      return bind_error_expression();
    }
  }

  return new BoundVariableReferenceExpressionNode(variable);
}

BoundExpressionNode *Binder::bind_ternary_expression(
    Syntax::TernaryExpressionNode *ternary_expression) {
  auto bound_conditional =
      bind_expression(ternary_expression->conditional_expression());
  if (!Symbols::TypeSymbol::is_conditional_type(bound_conditional->type())) {
    m_diagnostics->report_integer_type_required(
        ternary_expression->token(), bound_conditional->type()->str());
    return bind_error_expression();
  }

  auto bound_left =
      bind_expression(ternary_expression->true_expression(), true);
  auto bound_right =
      bind_expression(ternary_expression->false_expression(), true);
  auto left_type = bound_left->type();
  auto right_type = bound_right->type();

  if (Symbols::TypeSymbol::is_error_type(left_type) ||
      Symbols::TypeSymbol::is_error_type(right_type))
    return bind_error_expression();

  if (!Symbols::TypeSymbol::are_types_equivalent(left_type, right_type) &&
      !Symbols::TypeSymbol::are_types_equivalent(right_type, left_type)) {

    m_diagnostics->report_incompatible_operand_types(
        ternary_expression->token(), left_type->str(), right_type->str());
    return bind_error_expression();
  }

  auto ternary_ret_type =
      Symbols::TypeSymbol::get_wider_type(left_type, right_type);

  return new BoundTernaryExpressionNode(ternary_ret_type, bound_conditional,
                                        bound_left, bound_right);
}

BoundExpressionNode *
Binder::bind_unary_expression(Syntax::UnaryExpressionNode *unary_expression) {
  auto bound_expression = bind_expression(unary_expression->expression());

  if (Symbols::TypeSymbol::is_error_type(bound_expression->type()))
    return bind_error_expression();

  auto bound_operator = BoundUnaryOperatorNode::Bind(
      unary_expression->syntax_token_type(), bound_expression->type());

  if (bound_operator == nullptr) {
    m_diagnostics->report_invalid_unary_operator(
        unary_expression->token(), bound_expression->type()->str());
    return bind_error_expression();
  }

  return new BoundUnaryExpressionNode(bound_operator, bound_expression);
}

} // namespace Binding