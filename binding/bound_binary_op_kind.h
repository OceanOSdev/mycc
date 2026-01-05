#pragma once
#ifndef BOUND_BINARY_OP_KIND_H
#define BOUND_BINARY_OP_KIND_H

namespace Binding {

enum BoundBinaryOpKind : int {
  /* I Types */
  Modulo,
  BitwiseAnd,
  BitwiseOr,

  /* N types */
  Addition,
  Subtraction,
  Multiplication,
  Division,

  /* N types (but are comparison ops) */
  Equals,
  NotEquals,
  LessThan,
  LessThanOrEquals,
  GreaterThan,
  GreaterThanOrEquals,
  LogicalAnd,
  LogicalOr
};

}

#endif