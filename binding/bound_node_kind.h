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

    // Statements
    ExpressionStatement,
    BlockStatement,
    ReturnStatement,
    VariableDeclaration,
    VariableGroupDeclaration,
    StructDeclaration,

    // Globals
    GlobalStatement,
    FunctionDefinition
};

using BoundNodeKind = enum BoundNodeKind;

}

#endif