#ifndef BASICBLOCK_HPP
#define BASICBLOCK_HPP

#include <vector>

#include "Instruction.hpp"

class Graph;

struct BasicBlock {
    std::vector<BasicBlock*> preds_;
    std::vector<BasicBlock*> succs_;
    Instruction* firstPhi_;
    Instruction* firstInst_;
    Instruction* lastInst_;
    Graph* graph_;

    BasicBlock(Graph* graph);
    void addInstruction(Instruction* inst);
    void addInstruction(const std::vector<Instruction*>& insts);
    void addSuccessor(BasicBlock* succ);
    void addPredecessor(BasicBlock* pred);

    std::vector<size_t> getInstructionIds();
};

#endif  // BASICBLOCK_HPP