#ifndef BASICBLOCK_HPP
#define BASICBLOCK_HPP

#include <vector>

#include "Instruction.hpp"

namespace Compiler {

class Graph;

struct BasicBlock {
    std::vector<BasicBlock*> preds_;
    BasicBlock* firstSuccs_;
    BasicBlock* secondSuccs_;
    Instruction* firstPhi_;
    std::vector<Instruction*> insts_;
    Graph* graph_;
    size_t id_;
    std::vector<size_t> loopIds_;

    BasicBlock(size_t id, Graph* graph);
    void addInstructionToBegin(Instruction* inst);
    void addInstructionToBegin(const std::vector<Instruction*>& insts);
    void addInstructionToEnd(Instruction* inst);
    void addInstructionToEnd(const std::vector<Instruction*>& insts);
    void addInstruction(Instruction* inst, size_t pos);
    void addInstruction(const std::vector<Instruction*>& insts, size_t pos);
    void addSuccessor(BasicBlock* succ, bool succsType);
    std::vector<size_t> getInstructionIds();

private:
    void addPredecessor(BasicBlock* pred);
};

}  // namespace Compiler

#endif  // BASICBLOCK_HPP