#pragma once
#ifndef CONTROL_FLOW_GRAPH_FACTORY_H
#define CONTROL_FLOW_GRAPH_FACTORY_H

#include <unordered_map>
#include <vector>

namespace Binding {
    class BoundExpressionNode;
    class BoundStatementNode;
    class BoundLabel;
}

namespace FlowAnalysis {
class BasicBlock;
class BasicBlockBranch;
class ControlFlowGraph;

class ControlFlowGraphFactory {
private:
    std::unordered_map<Binding::BoundStatementNode*, BasicBlock*> m_block_from_statement_map;
    std::unordered_map<Binding::BoundLabel*, BasicBlock*> m_block_from_label_map;
    std::vector<BasicBlockBranch*> m_branches;
    BasicBlock* m_start_block;
    BasicBlock* m_end_block;

    /**
     * @brief Connect two basic blocks together.
     * 
     * If the jump is conditional, pass the conditional expression
     * in as the condition parameter.
     * 
     * @param from The block to jump from.
     * @param to  The block to jump to.
     * @param condition The condition for which the jump should happen.
     */
    void connect_blocks(BasicBlock* from, BasicBlock* to, Binding::BoundExpressionNode* condition = nullptr);

    /**
     * @brief Remove unreachable block from block list.
     * 
     * @param block_list The list of blocks that contains unreachable code.
     * @param block_to_remove The block that represents unreachable code.
     */
    void remove_block(std::vector<BasicBlock*> block_list, BasicBlock* block_to_remove);
public:
    /**
     * @brief Creates a new Control Flow Graph Factory.
     */
    ControlFlowGraphFactory();

    /**
     * @brief Builds the control flow graph of the given block group.
     * 
     * @param blocks The basic blocks in the control flow diagram.
     * @return A graph modeling the control flow of the block group.
     */
    ControlFlowGraph* build(std::vector<BasicBlock*> blocks);
};

}

#endif