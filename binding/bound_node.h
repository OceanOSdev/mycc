#pragma once
#ifndef BOUND_NODE_H
#define BOUND_NODE_H

#include "bound_node_kind.h"

namespace Binding {

class BoundNode {
public:
    BoundNode() {}
    virtual ~BoundNode() = default;

    /*
     * An enum representing the kind of bound node this is
     */
    virtual BoundNodeKind kind() const = 0;
};

}

#endif