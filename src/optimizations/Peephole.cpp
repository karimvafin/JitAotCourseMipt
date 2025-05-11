#include <iostream>
#include <assert.h>

#include "optimizations/Peephole.hpp"

namespace Compiler {

void peepholeInstruction(Instruction* inst, Graph* graph) {
    if (inst->opCode_ == OpCode::CNST) {
        return;
    } else if (inst->opCode_ == OpCode::ADD) {
        assert(inst->inputs_.size() == 2);
        Instruction* left = inst->inputs_[0];
        Instruction* right = inst->inputs_[1];

        // #1 Add zero
        if (left->value_->isZero()) {
            inst->opCode_ = OpCode::MOV;
            inst->deleteInput(left);
        } else if (right->value_->isZero()) {
            inst->opCode_ = OpCode::MOV;
            inst->deleteInput(right);
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
        if (right->opCode_ == OpCode::CNST) {
            assert(right->value_);
            if (right->value_->isZero()) {
                inst->opCode_ = OpCode::MOV;
                inst->deleteInput(right);
                return;
            }

            // #2 Large shift
            if (right->value_->greaterThan(getInstTypeSize(left->type_))) {
                Value* zero = graph->createValue(0, inst->type_);
                graph->makeInstructionConst(inst, zero);
                inst->deleteInput(right);
                return;
            }
        }
        
    } else if (inst->opCode_ == OpCode::OR) {
        assert(inst->inputs_.size() == 2);
        Instruction* left = inst->inputs_[0];
        Instruction* right = inst->inputs_[1];

        // #1 Compare with zero
        if (left->opCode_ == OpCode::CNST && left->value_->isZero()) {
            inst->opCode_ = OpCode::MOV;
            inst->deleteInput(left);
            return;
        }

        if (right->opCode_ == OpCode::CNST && right->value_->isZero()) {
            inst->opCode_ = OpCode::MOV;
            inst->deleteInput(right);
            return;
        }

        // #2 Compare with itself
        if (left == right) {
            Value* one = graph->createValue(1, inst->type_);
            graph->makeInstructionConst(inst, one);
            inst->deleteInput(left);
            inst->deleteInput(right);
            return;
        }
    }
}

void peepholeOptimization(Graph* graph) {
    for (auto &b: graph->blocks()) {
        for (auto &i: b->insts_) {
            peepholeInstruction(i, graph);
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

