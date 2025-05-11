#include <iostream>
#include "assert.h"
#include "optimizations/ConstantFolding.hpp"

namespace Compiler {

void constantFoldingInstruction(Instruction* inst) {
    if (inst->opCode_ == OpCode::CNST) {
        return;
    } else if (inst->opCode_ == OpCode::ADD || 
               inst->opCode_ == OpCode::SHR ||
               inst->opCode_ == OpCode::OR) {
        assert(inst->inputs_.size() == 2);
        Instruction* left = inst->inputs_[0];
        Instruction* right = inst->inputs_[1];
        constantFoldingInstruction(left);
        constantFoldingInstruction(right);
        if (left->opCode_ == OpCode::CNST && right->opCode_ == OpCode::CNST) {
            inst->opCode_ = OpCode::CNST;
            inst->clearInput();
        }
    }

}

void constantFolding(Graph* graph) {
    for (auto &b: graph->blocks()) {
        for (auto &i: b->insts_) {
            constantFoldingInstruction(i);
        }
    }
    for (auto &b: graph->blocks()) {
        auto ne = remove_if(b->insts_.begin(), b->insts_.end(),
                        [](Instruction* inst) {
                          return inst->opCode_ == OpCode::CNST && inst->users_.empty();
                        });
        b->insts_.erase(ne, b->insts_.end());
    }
}

} // namespace Compiler

