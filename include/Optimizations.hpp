#ifndef OPTIMIZATIONS_HPP
#define OPTIMIZATIONS_HPP

#include <assert.h>
#include <iostream>

#include "Instruction.hpp"
#include "Graph.hpp"

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
    for (auto &b: graph->blocks_) {
        for (auto &i: b->insts_) {
            constantFoldingInstruction(i);
        }
    }
    for (auto &b: graph->blocks_) {
        auto ne = remove_if(b->insts_.begin(), b->insts_.end(),
                        [](Instruction* inst) {
                          return inst->opCode_ == OpCode::CNST && inst->users_.empty();
                        });
        b->insts_.erase(ne, b->insts_.end());
    }
}

void peepholeInstruction(Instruction* inst) {
    if (inst->opCode_ == OpCode::CNST) {
        return;
    } else if (inst->opCode_ == OpCode::ADD) {
        assert(inst->inputs_.size() == 2);
        Instruction* left = inst->inputs_[0];
        Instruction* right = inst->inputs_[1];

        // #1 Add zero
        if (left->value_.IsZero()) {
            inst->opCode_ = OpCode::MOV;
            inst->deleteInput(left);
        }
        if (right->value_.IsZero()) {
            inst->opCode_ = OpCode::MOV;
            inst->deleteInput(right);
        }
        if (inst->inputs_.empty()) {
            inst->opCode_ = OpCode::CNST;
            inst->value_.SetZero();
        }

        // #2 Commutative simplification
        if (inst->inputs_.size() == 2)
            if (left->id_ > right->id_) {
                inst->inputs_[0] = right;
                inst->inputs_[1] = left;
            }

    } else if (inst->opCode_ == OpCode::SHR) {
        assert(inst->inputs_.size() == 2);
        Instruction* left = inst->inputs_[0];
        Instruction* right = inst->inputs_[1];

        // #1 Shift by zero
        if (right->value_.IsZero()) {
            inst->opCode_ = OpCode::MOV;
            inst->deleteInput(right);
            return;
        }

        // #2 Large shift
        if (right->value_.GreaterThan(left->value_)) {
            inst->opCode_ = OpCode::CNST;
            inst->deleteInput(right);
            inst->value_.SetZero();
            return;
        }

    } else if (inst->opCode_ == OpCode::OR) {
        assert(inst->inputs_.size() == 2);
        Instruction* left = inst->inputs_[0];
        Instruction* right = inst->inputs_[1];

        // #1 Compare with zero
        if (left->value_.IsZero() || right->value_.IsZero()) {
            inst->opCode_ = OpCode::CNST;
            inst->deleteInput(right);
            inst->value_.SetZero();
            return;
        }

        // #2 Compare with itself
        if (left->value_.Compare(right->value_)) {
            inst->opCode_ = OpCode::CNST;
            inst->deleteInput(right);
            inst->value_.SetOne();
            return;
        }
    }
}

void peepholeOptimization(Graph* graph) {
    for (auto &b: graph->blocks_) {
        for (auto &i: b->insts_) {
            peepholeInstruction(i);
        }
    }
    for (auto &b: graph->blocks_) {
        auto ne = remove_if(b->insts_.begin(), b->insts_.end(),
                        [](Instruction* inst) {
                          return inst->opCode_ == OpCode::CNST && inst->users_.empty();
                        });
        b->insts_.erase(ne, b->insts_.end());
    }
}

} // namespace Compiler

#endif // OPTIMIZATIONS_HPP