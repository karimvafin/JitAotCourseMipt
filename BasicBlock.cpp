#include <iostream>

#include "BasicBlock.hpp"

BasicBlock::BasicBlock(Graph* graph) : firstPhi_(nullptr), firstInst_(nullptr), lastInst_(nullptr), graph_(graph) {}

void BasicBlock::addInstruction(Instruction* inst) {
    if (!firstInst_) {
        inst->prev_ = nullptr;
        inst->next_ = nullptr;
        firstInst_ = inst;
        lastInst_ = inst;
        return;
    }
    lastInst_->next_ = inst;
    inst->prev_ = lastInst_;
    lastInst_ = inst;
}

void BasicBlock::addInstruction(const std::vector<Instruction*>& insts) {
    const size_t numInsts = insts.size();
    if (numInsts == 0) return;
    if (!firstInst_) {
        insts[0]->prev_ = nullptr;
        insts[0]->next_ = nullptr;
        firstInst_ = insts[0];
        lastInst_ = insts[0];
    } else {
        lastInst_->next_ = insts[0];
        insts[0]->prev_ = lastInst_;
    }
    for (size_t i = 1; i < numInsts; ++i) {
        insts[i - 1]->next_ = insts[i];
        insts[i]->prev_ = insts[i - 1];
    }
    insts[numInsts - 1]->next_ = nullptr;
    lastInst_ = insts[numInsts - 1];
}

void BasicBlock::addPredecessor(BasicBlock* pred) {
    preds_.push_back(pred);
}

void BasicBlock::addSuccessor(BasicBlock* succ) {
    if (succs_.size() == 2) {   
        std::cout << "Basic Block cannot contain more than 2 succs!" << std::endl;
        return;
    }
    succs_.push_back(succ);
}

std::vector<size_t> BasicBlock::getInstructionIds() {
    std::vector<size_t> res;
    Instruction* ptr = firstInst_;
    while (ptr) {
        res.push_back(ptr->id_);
        ptr = ptr->next_;
    }
    return res;
}