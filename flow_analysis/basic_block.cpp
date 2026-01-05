#include <algorithm>

#include "../binding/bound_statement_node.h"
#include "basic_block.h"

namespace FlowAnalysis {

BasicBlock::BasicBlock() : m_is_start(false), m_is_end(false) {}

BasicBlock::BasicBlock(bool is_start)
    : m_is_start(is_start), m_is_end(!is_start) {}

bool BasicBlock::is_start() const { return m_is_start; }

bool BasicBlock::is_end() const { return m_is_end; }

std::vector<Binding::BoundStatementNode *> BasicBlock::statements() const {
  return m_statements;
}

void BasicBlock::add_statement(Binding::BoundStatementNode *statement) {
  m_statements.push_back(statement);
}

void BasicBlock::add_all_statements(
    std::vector<Binding::BoundStatementNode *> statements) {
  m_statements.insert(m_statements.end(), statements.begin(), statements.end());
}

std::vector<BasicBlockBranch *> BasicBlock::incoming() const {
  return m_incoming;
}

void BasicBlock::remove_incoming(BasicBlockBranch *incoming_block_branch) {
  auto end_iter =
      std::remove(m_incoming.begin(), m_incoming.end(), incoming_block_branch);
  m_incoming.erase(end_iter, m_incoming.end());
}

void BasicBlock::add_incoming(BasicBlockBranch *incoming_block) {
  m_incoming.push_back(incoming_block);
}

void BasicBlock::add_all_incoming(
    std::vector<BasicBlockBranch *> incoming_blocks) {
  m_incoming.insert(m_incoming.end(), incoming_blocks.begin(),
                    incoming_blocks.end());
}

std::vector<BasicBlockBranch *> BasicBlock::outgoing() const {
  return m_outgoing;
}

void BasicBlock::remove_outgoing(BasicBlockBranch *outgoing_block_branch) {
  auto end_iter =
      std::remove(m_outgoing.begin(), m_outgoing.end(), outgoing_block_branch);
  m_outgoing.erase(end_iter, m_outgoing.end());
}

void BasicBlock::add_outgoing(BasicBlockBranch *outgoing_block) {
  m_outgoing.push_back(outgoing_block);
}

void BasicBlock::add_all_outgoing(
    std::vector<BasicBlockBranch *> outgoing_blocks) {
  m_outgoing.insert(m_outgoing.end(), outgoing_blocks.begin(),
                    outgoing_blocks.end());
}

} // namespace FlowAnalysis