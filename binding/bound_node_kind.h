#pragma once
#ifndef BOUND_NODE_KIND_H
#define BOUND_NODE_KIND_H

namespace Binding {

enum BoundNodeKind {
  // Expressions
  CastExpression,
  BinaryExpression,
  ErrorExpression,
  LiteralExpression,
  IndexExpression,
  MemberAccessExpression,
  VariableReferenceExpression,
  AssignmentExpression,
  UnaryExpression,
  CallExpression,
  TernaryExpression,
  IncrementExpression,
  DecrementExpression,

  // Statements
  ExpressionStatement,
  BlockStatement,
  ReturnStatement,
  VariableDeclaration,
  VariableGroupDeclaration,
  StructDeclaration,
  EmptyStatement,
  LabelStatement,
  IfStatement,
  GotoStatement,
  ConditionalGotoStatement,
  ForStatement,
  WhileStatement,
  DoWhileStatement,

  // Globals
  GlobalStatement,
  FunctionDefinition
};

using BoundNodeKind = enum BoundNodeKind;

} // namespace Binding

#endif