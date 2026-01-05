#include "basic_block_branch.h"
#include "../binding/bound_expression_node.h"
#include "basic_block.h"

namespace FlowAnalysis {

BasicBlockBranch::BasicBlockBranch(BasicBlock *from, BasicBlock *to,
                                   Binding::BoundExpressionNode *condition)
    : m_from(from), m_to(to), m_condition(condition) {}

BasicBlock *BasicBlockBranch::from() const { return m_from; }

BasicBlock *BasicBlockBranch::to() const { return m_to; }

Binding::BoundExpressionNode *BasicBlockBranch::condition() const {
  return m_condition;
}

bool BasicBlockBranch::is_conditional() const { return m_condition != nullptr; }

} // namespace FlowAnalysis