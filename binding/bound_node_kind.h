#pragma once
#ifndef BOUND_NODE_KIND_H
#define BOUND_NODE_KIND_H

namespace Binding {

enum BoundNodeKind {
    CastExpression,
    BinaryExpression,
    ErrorExpression
};

using BoundNodeKind = enum BoundNodeKind;

}

#endif