#include <iostream>

#include "BasicBlock.hpp"

namespace Compiler {

BasicBlock::BasicBlock(size_t id, Graph* graph) : id_(id), firstPhi_(nullptr), graph_(graph), firstSuccs_(nullptr), secondSuccs_(nullptr) {}

void BasicBlock::addInstructionToBegin(Instruction* inst) {
    insts_.insert(insts_.begin(), inst);
}

void BasicBlock::addInstructionToBegin(const std::vector<Instruction*>& insts) {
    insts_.insert(insts_.begin(), insts.begin(), insts.end());
}

void BasicBlock::addInstructionToEnd(Instruction* inst) {
    insts_.push_back(inst);
}

void BasicBlock::addInstructionToEnd(const std::vector<Instruction*>& insts) {
    insts_.insert(insts_.end(), insts.begin(), insts.end());
}

void BasicBlock::addInstruction(Instruction* inst, size_t pos) {
    if (pos >= insts_.size()) {
        std::cout << "Invalid position! Position: " << pos << ". Vector size: " << insts_.size() << std::endl;
        return;
    }
    insts_.insert(insts_.begin() + pos, inst);
}

void BasicBlock::addInstruction(const std::vector<Instruction*>& insts, size_t pos) {
    if (pos >= insts_.size()) {
        std::cout << "Invalid position! Position: " << pos << ". Vector size: " << insts_.size() << std::endl;
        return;
    }
    insts_.insert(insts_.begin() + pos, insts.begin(), insts.end());
}

void BasicBlock::addPredecessor(BasicBlock* pred) {
    preds_.push_back(pred);
}

void BasicBlock::addSuccessor(BasicBlock* succ, bool succsType) {
    BasicBlock*& succsPtr = succsType ? firstSuccs_ : secondSuccs_;
    succsPtr = succ;
    succ->addPredecessor(this);
}

std::vector<size_t> BasicBlock::getInstructionIds() {
    std::vector<size_t> res;
    res.reserve(insts_.size());
    for (auto elem : insts_) {
        res.push_back(elem->id_);
    }
    return res;
}

}  // namespace Compiler