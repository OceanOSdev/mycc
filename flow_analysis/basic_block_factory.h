#pragma once
#ifndef BASIC_BLOCK_FACTORY_H
#define BASIC_BLOCK_FACTORY_H

#include <vector>

namespace Binding {
    class BoundStatementNode;
    class BoundBlockStatementNode;
}

namespace FlowAnalysis {
class BasicBlock;

class BasicBlockFactory {
private:
    std::vector<Binding::BoundStatementNode*> m_statements;
    std::vector<BasicBlock*> m_blocks;
    void start_block();
    void end_block();
public:
    /**
     * @brief Creates a new basic block factory. 
     */
    BasicBlockFactory();

    /**
     * @brief Builds a basic block list.
     * 
     * The intended use case for this builder is to 
     * create a list of basic blocks that are contained
     * inside the body of a method.
     * 
     * @param block The bounded block statement to build block list from. 
     * @return The list of basic blocks that make up a bounded block statement.
     */
    std::vector<BasicBlock*> build(Binding::BoundBlockStatementNode* block);
};

}

#endif