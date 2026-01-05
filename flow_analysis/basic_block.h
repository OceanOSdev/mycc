#pragma once
#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H

#include <vector>

namespace Binding {
class BoundStatementNode;
}

namespace FlowAnalysis {
class BasicBlockBranch;

class BasicBlock {
private:
  bool m_is_start;
  bool m_is_end;
  std::vector<Binding::BoundStatementNode *> m_statements;
  std::vector<BasicBlockBranch *> m_incoming;
  std::vector<BasicBlockBranch *> m_outgoing;

public:
  /**
   * @brief Constructs a new instance of a Basic Block in the Control Flow
   * Diagram.
   */
  BasicBlock();

  /**
   * @brief Constructs a new instance of a Basic Block in the Control Flow
   * Diagram.
   *
   * @param is_start Whether this block is the start block in the graph.
   */
  BasicBlock(bool is_start);

  /**
   * @brief Whether this block is the start block in the graph.
   *
   * @return Whether this block is the start block in the graph.
   */
  bool is_start() const;

  /**
   * @brief Whether this block is the start block in the graph.
   *
   * @return Whether this block is the end block in the graph.
   */
  bool is_end() const;

  /**
   * @brief The Bound Statement Nodes in this basic block.
   *
   * @return A vector containing BoundStatementNodes
   */
  std::vector<Binding::BoundStatementNode *> statements() const;

  /**
   * @brief Add a statement to the basic block.
   *
   * @param statement The statement to add.
   */
  void add_statement(Binding::BoundStatementNode *statement);

  /**
   * @brief Add a range of statements to the basic block.
   *
   * @param statements The list of statements to add.
   */
  void
  add_all_statements(std::vector<Binding::BoundStatementNode *> statements);

  /**
   * @brief The incoming branches into this block.
   *
   * @return A vector containing the incoming branches.
   */
  std::vector<BasicBlockBranch *> incoming() const;

  /**
   * @brief Removes an incoming block branch from this basic block.
   *
   * @param incoming_block_branch The incoming branch to remove.
   */
  void remove_incoming(BasicBlockBranch *incoming_block_branch);

  /**
   * @brief Add an incoming block branch to this basic block.
   *
   * @param incoming_block_branch The incoming basic block branch edge to add.
   */
  void add_incoming(BasicBlockBranch *incoming_block_branch);

  /**
   * @brief Add a range of incoming block branches to this basic block.
   *
   * @param incoming_block_branches The list of incoming basic block branch
   * edges to add.
   */
  void
  add_all_incoming(std::vector<BasicBlockBranch *> incoming_block_branches);

  /**
   * @brief The outgoing branches from this block.
   *
   * @return A vector containing the outgoing branches.
   */
  std::vector<BasicBlockBranch *> outgoing() const;

  /**
   * @brief Removes an outgoing block branch from this basic block.
   *
   * @param outgoing_block_branch The outgoing branch to remove.
   */
  void remove_outgoing(BasicBlockBranch *outgoing_block_branch);

  /**
   * @brief Add an outgoing block branch to this basic block.
   *
   * @param outgoing_block_branch The outgoing basic block branch edge to add.
   */
  void add_outgoing(BasicBlockBranch *outgoing_block_branch);

  /**
   * @brief Add a range of outgoing block branches to this basic block.
   *
   * @param outgoing_block_branches The list of outgoing basic block branch
   * edges to add.
   */
  void
  add_all_outgoing(std::vector<BasicBlockBranch *> outgoing_block_branches);
};

} // namespace FlowAnalysis

#endif