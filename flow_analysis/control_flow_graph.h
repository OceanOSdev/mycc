#pragma once
#ifndef CONTROL_FLOW_GRAPH_H
#define CONTROL_FLOW_GRAPH_H

#include <vector>

namespace Binding {
    class BoundBlockStatementNode;
}

namespace FlowAnalysis {
class BasicBlock;
class BasicBlockBranch;


class ControlFlowGraph {
private:
    BasicBlock* m_start_block;
    BasicBlock* m_end_block;
    std::vector<BasicBlock*> m_block_list;
    std::vector<BasicBlockBranch*> m_branches;

    ControlFlowGraph(BasicBlock* start, BasicBlock* end, std::vector<BasicBlock*> blocks, std::vector<BasicBlockBranch*> branches);
    friend class ControlFlowGraphFactory;
public:

    /**
     * @brief The entry node of the control flow graph.
     * 
     * @return The start block.
     */
    BasicBlock* start_block() const;

    /**
     * @brief The exit node of the control flow graph.
     * 
     * @return The end block.
     */
    BasicBlock* end_block() const;

    /**
     * @brief The list of blocks in this control flow graph.
     * 
     * @return A vector containing the blocks in this CFG.
     */
    std::vector<BasicBlock*> block_list() const;

    /**
     * @brief The edges of this graph.
     * 
     * @return The branches in this CFG. 
     */
    std::vector<BasicBlockBranch*> branches() const;

    /**
     * @brief Creates a control flow graph for a given block statement.
     * 
     * @param method_body The block statement to create a control flow graph for.
     * @return The control flow graph representing the given method body.
     */
    static ControlFlowGraph* create(Binding::BoundBlockStatementNode* method_body);

    /**
     * @brief Determines whether or not all computation paths have a return statement for a given block statement.
     * 
     * @param method_body The block statement to check.
     * @return If all paths in the code body have a return statement.
     */
    static bool all_paths_in_DAG_return(Binding::BoundBlockStatementNode* method_body);
};

}

#endif