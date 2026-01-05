#include "control_flow_graph.h"
#include "basic_block.h"
#include "basic_block_branch.h"
#include "basic_block_factory.h"
#include "control_flow_graph_factory.h"

#include "../binding/bound_block_statement_node.h"
#include "../binding/bound_node_kind.h"

namespace FlowAnalysis {

ControlFlowGraph::ControlFlowGraph(BasicBlock *start, BasicBlock *end,
                                   std::vector<BasicBlock *> blocks,
                                   std::vector<BasicBlockBranch *> branches)
    : m_start_block(start), m_end_block(end), m_block_list(blocks),
      m_branches(branches) {}

BasicBlock *ControlFlowGraph::start_block() const { return m_start_block; }

BasicBlock *ControlFlowGraph::end_block() const { return m_end_block; }

std::vector<BasicBlock *> ControlFlowGraph::block_list() const {
  return m_block_list;
}

std::vector<BasicBlockBranch *> ControlFlowGraph::branches() const {
  return m_branches;
}

ControlFlowGraph *
ControlFlowGraph::create(Binding::BoundBlockStatementNode *method_body) {
  auto basic_block_factory = new BasicBlockFactory();
  auto block_list = basic_block_factory->build(method_body);

  auto graph_factory = new ControlFlowGraphFactory();
  return graph_factory->build(block_list);
}

bool ControlFlowGraph::all_paths_in_DAG_return(
    Binding::BoundBlockStatementNode *method_body) {
  auto graph = create(method_body);

  for (auto branch : graph->end_block()->incoming()) {
    if (branch->from()->statements().empty())
      return false;
    auto last_statement = branch->from()->statements().back();
    if (last_statement->kind() != Binding::BoundNodeKind::ReturnStatement)
      return false;
  }
  return true;
}

} // namespace FlowAnalysis