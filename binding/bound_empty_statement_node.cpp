#include "bound_empty_statement_node.h"

namespace Binding {

BoundEmptyStatementNode::BoundEmptyStatementNode() {}

/*
 * An enum representing the kind of bound node this is
 */
BoundNodeKind BoundEmptyStatementNode::kind() const {
    return BoundNodeKind::EmptyStatement;
}

}