#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "BasicBlock.hpp"

struct Graph {
    std::vector<BasicBlock*> blocks_;

    void addBasicBlock(BasicBlock* block);
    void addBasicBlock(const std::vector<BasicBlock*>& blocks);
};

#endif  // GRAPH_HPP