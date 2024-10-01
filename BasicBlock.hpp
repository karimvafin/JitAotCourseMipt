#ifndef BASICBLOCK_HPP
#define BASICBLOCK_HPP

#include <vector>

#include "Instruction.hpp"
#include "Graph.hpp"

struct BasicBlock {
    std::vector<BasicBlock*> preds_;
    std::vector<BasicBlock*> succs_;
    Instruction* firstPhi_;
    Instruction* firstInst_;
    Instruction* lastInst_;
    Graph* graph_;
};

#endif  // BASICBLOCK_HPP