#pragma once
#ifndef BOUND_NODE_KIND_H
#define BOUND_NODE_KIND_H

namespace Binding {

enum BoundNodeKind {
    // Expressions
    CastExpression,
    BinaryExpression,
    ErrorExpression,

    // Statements
    ExpressionStatement,
    BlockStatement,

    // Globals
    GlobalStatement,
    FunctionDefinition
};

using BoundNodeKind = enum BoundNodeKind;

}

#endif