#include <iostream>

#include "BasicBlock.hpp"

namespace Compiler {

BasicBlock::BasicBlock(Graph* graph) : firstPhi_(nullptr), graph_(graph) {}

void BasicBlock::addInstructionToBegin(Instruction* inst) {
    insts_.insert(insts_.begin(), inst);
}

void BasicBlock::addInstructionToBegin(const std::vector<Instruction*>& insts) {
    insts_.insert(insts_.begin(), insts.begin(), insts.end());
}

void BasicBlock::addInstructionToEnd(Instruction* inst) {
    if (inst->opCode_ == OpCode::PHI) {
        phiInsts_.push_back(inst);
        return;
    }
    insts_.push_back(inst);
}

void BasicBlock::addInstructionToEnd(const std::vector<Instruction*>& insts) {
    for (auto inst: insts) {
        if (inst->opCode_ == OpCode::PHI) {
            phiInsts_.push_back(inst);
        } else {
            insts_.push_back(inst);
        }
    }
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

void BasicBlock::removeInstruction(Instruction* inst) {
    insts_.erase(std::remove(insts_.begin(), insts_.end(), inst));
}

void BasicBlock::addPredecessor(BasicBlock* pred) {
    preds_.push_back(pred);
}

void BasicBlock::removePredecessor(BasicBlock* pred) {
    auto predIt = std::find(preds_.begin(), preds_.end(), pred);
    if (predIt == preds_.end()) return;
    preds_.erase(predIt);
}

void BasicBlock::addSuccessor(BasicBlock* succ, bool succsType) {
    if (succ == nullptr) return;
    BasicBlock*& succsPtr = succsType ? firstSuccs_ : secondSuccs_;
    succsPtr = succ;
    succ->addPredecessor(this);
}

void BasicBlock::removeFirstSuccessor() {
    if (firstSuccs_ == nullptr) return;
    firstSuccs_->removePredecessor(this);
    firstSuccs_ = nullptr;
}

void BasicBlock::removeSecondSuccessor() {
    if (secondSuccs_ == nullptr) return;
    secondSuccs_->removePredecessor(this);
    secondSuccs_ = nullptr;
}

std::vector<size_t> BasicBlock::getInstructionIds() const {
    std::vector<size_t> res;
    res.reserve(phiInsts_.size() + insts_.size());
    for (auto elem : phiInsts_) {
        res.push_back(elem->id_);
    }
    for (auto elem : insts_) {
        res.push_back(elem->id_);
    }
    return res;
}

bool BasicBlock::hasInstruction(size_t instId) const {
    for (auto inst: insts_) {
        if (inst->id_ == instId) return true;
    }
    return false;
}

}  // namespace Compiler
