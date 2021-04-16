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

    // Statements
    ExpressionStatement,
    BlockStatement,
    ReturnStatement,
    VariableDeclaration,
    VariableGroupDeclaration,

    // Globals
    GlobalStatement,
    FunctionDefinition
};

using BoundNodeKind = enum BoundNodeKind;

}

#endif