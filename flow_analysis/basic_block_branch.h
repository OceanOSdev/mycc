#pragma once
#ifndef BASIC_BLOCK_BRANCH_H
#define BASIC_BLOCK_BRANCH_H

namespace Binding {
    class BoundExpressionNode;
}

namespace FlowAnalysis {
class BasicBlock;

class BasicBlockBranch {
private:
    BasicBlock* m_from;
    BasicBlock* m_to;
    Binding::BoundExpressionNode* m_condition;
public:
    /**
     * @brief Creates a new branch from one Basic Block to another.
     * 
     * @param from The source block.
     * @param to  The destination block.
     * @param condition The (optional (kinda)) condition to jump.
     */
    BasicBlockBranch(BasicBlock* from, BasicBlock* to, Binding::BoundExpressionNode* condition);

    /**
     * @brief The basic block that this branch originates from.
     * 
     * @return The source block.
     */
    BasicBlock* from() const;

    /**
     * @brief The basic block that the source branch jumps to.
     * 
     * @return The destination block.
     */
    BasicBlock* to() const;

    /**
     * @brief The conditional expression that causes the source
     *        block to jump to the destination block.
     * 
     * @return The bound expression node representing the conditional jump. 
     */
    Binding::BoundExpressionNode* condition() const;

    /**
     * @brief Whether this branch is conditional (i.e. was this a conditional goto).
     * 
     * @return Whether or not the conditional expression is null.
     */
    bool is_conditional() const;
};

}

#endif