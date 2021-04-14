#pragma once
#ifndef SYNTAX_KIND_H
#define SYNTAX_KIND_H

namespace Syntax {

enum SyntaxKind : int {
    // Expressions
    AssignmentExpression,
    BinaryExpression,
    CallExpression,
    CastExpression,
    DecrementExpression,
    IncrementExpression,
    IndexExpression,
    LiteralValExpression,
    MemberExpression,
    NameExpression,
    TernaryExpression,
    UnaryExpression,

    // Statements
    BlockStatement,
    BreakStatement,
    ContinueStatement,
    DoWhileStatement,
    ExpressionStatement,
    ForStatement,
    StructDeclaration,
    VariableDeclaration,
    IfStatement,
    ReturnStatement,
    WhileStatement,

    // Nodes
    FormalParameter,
    FunctionDeclaration,
    FunctionDefinition,
    FunctionPrototype,
    GlobalVariableDeclaration,
    GlobalStructDeclaration,
    TranslationUnit,
    Program,

    // Unused
    PartialVariableDeclaration
};

}

#endif