#include <exception>

#include "optimizations/StaticInline.hpp"
#include "RPO.hpp"
#include "Function.hpp"

namespace Compiler {

int instructionCountThreshold = 100;

void splitBlockByInst(BasicBlock* block, BasicBlock* residualBlock, Instruction* inst) {
    const auto instIt = std::find(block->insts_.begin(), block->insts_.end(), inst);
    if (instIt == block->insts_.end()) {
        throw std::runtime_error("No such instruction in basic block!");
    }
    for (auto it = std::next(instIt), ite = block->insts_.end(); it != ite; ++it) {
        residualBlock->addInstructionToEnd(*it);
    }
    for (size_t i = 0; i < residualBlock->insts_.size(); ++i) {
        block->insts_.pop_back();
    }
    BasicBlock* firstSuccs = block->firstSuccs_;
    BasicBlock* secondSuccs = block->secondSuccs_;
    block->removeFirstSuccessor();
    block->removeSecondSuccessor();
    block->addSuccessor(residualBlock, true);
    residualBlock->addSuccessor(firstSuccs, true);
    residualBlock->addSuccessor(secondSuccs, false);
}

void inlineCallee(Graph* callerGraph, BasicBlock* block, Instruction* callInst, Function* callee) {
    Graph* calleeGraph = callerGraph->copyCalleeGraph(callee->graph());

    // Split block with call instruction
    BasicBlock* residualBlock = callerGraph->createBasicBlock();
    splitBlockByInst(block, residualBlock, callInst);

    // Update parameters DataFlow
    
    BasicBlock* startBlockCallee = calleeGraph->startBlock();
    BasicBlock* startBlockCalleeSuccessor = startBlockCallee->firstSuccs_;
    auto& callParams = callInst->inputs_;
    std::vector<Instruction*> calleeParams;
    auto& calleeStartInsts = startBlockCallee->insts_;
    for (size_t i = 0; i < calleeStartInsts.size() && calleeStartInsts[i]->opCode_ == OpCode::PRM; ++i) {
        calleeParams.push_back(calleeStartInsts[i]);
    }
    assert(callParams.size() == calleeParams.size());
    for (size_t i = 0; i < calleeParams.size(); ++i) {
        auto calleeParam = calleeParams[i];
        for (auto user: calleeParam->users_) {
            user->swapInput(calleeParam, callParams[i]);
            // callInst->deleteInput(callParams[i]);
        }
        assert(calleeParam->users_.empty());
        startBlockCallee->removeInstruction(calleeParam);
    }
    
    // Update DataFlow for return
    std::vector<Instruction*> returnInsts;
    std::unordered_set<BasicBlock*> blocksRet;
    for (auto b: calleeGraph->blocks()) {
        for (auto i: b->insts_) {
            if (i->opCode_ == OpCode::RET) {
                returnInsts.push_back(i);
                blocksRet.insert(b);
            }
        }
    }

    if (blocksRet.size() > 0) {
        Instruction* callReplaceInst;
        if (blocksRet.size() > 1) {
            callReplaceInst = callerGraph->createInst(callee->retType_, OpCode::PHI);
        } else {
            callReplaceInst = callerGraph->createInst(callee->retType_, OpCode::MOV);
        }
        residualBlock->addInstructionToBegin(callReplaceInst);
        for (auto retInst: returnInsts) {
            assert(retInst->inputs_.size() == 1);
            for (auto retInput: retInst->inputs_) {
                retInst->deleteInput(retInput);
                callReplaceInst->addInput(retInput);
            }
        }

        for (auto user: callInst->users_) {
            user->swapInput(callInst, callReplaceInst);
        }
        for (auto b: blocksRet) {
            b->insts_.erase(std::remove_if(b->insts_.begin(), b->insts_.end(),
                                        [](Instruction* inst) {
                                            return inst->opCode_ == OpCode::RET;
                                            }));
        }
    }

    assert(callInst->users_.empty());
    // block->removeInstruction(callInst);

    // Move constants
    std::vector<Instruction*> startCalleeInsts = calleeGraph->startBlock()->insts_;
    for (auto i: startCalleeInsts) {
        if (i->opCode_ == OpCode::CNST) {
            callerGraph->addInstruction(i);
            callerGraph->startBlock()->addInstructionToEnd(i);
            calleeGraph->removeInstruction(i);
        }
    }
    // auto& startInsts = calleeGraph->startBlock()->insts_;
    // startInsts.erase(std::remove_if(startInsts.begin(), startInsts.end(),
    //                     [](Instruction* inst) {
    //                       return inst->opCode_ == OpCode::CNST;
    //                     }));

    // Move callee blocks and instructions to caller graph
    std::vector<BasicBlock*> calleeBlocks = calleeGraph->blocks();
    for (auto block: calleeBlocks) {
        if (block == calleeGraph->startBlock()) continue;
        callerGraph->addBasicBlock(block);
        for (auto i: block->insts_) {
            callerGraph->addInstruction(i);
            calleeGraph->removeInstruction(i);
        }
        calleeGraph->removeBasicBlock(block);
    }

    // Update ControlFlow: connect blocks
    for (auto blockRet: blocksRet) {
        assert(blockRet->firstSuccs_ == nullptr);
        assert(blockRet->secondSuccs_ == nullptr);
        blockRet->addSuccessor(residualBlock, true);
    }
    block->removeFirstSuccessor();
    block->addSuccessor(startBlockCalleeSuccessor, true);

    delete calleeGraph;
}

void staticInlineImpl(Graph* graph, std::unordered_set<Graph*>& processed) {
    processed.insert(graph);
    for (auto& b: graph->blocks()) {
        for (auto& i: b->insts_) {
            if (i->opCode_ == OpCode::CALL) {
                assert(i->callee_);
                auto calleeGraph = i->callee_->graph();
                assert(calleeGraph);
                if (processed.find(calleeGraph) != processed.end())
                    staticInlineImpl(calleeGraph, processed);
                if (calleeGraph->instructionCount() < instructionCountThreshold)
                    inlineCallee(graph, b, i, i->callee_);
            }
        }
    }
}

void staticInline(Graph* graph) {
    std::unordered_set<Graph*> processed;
    staticInlineImpl(graph, processed);
}

} // namespace Compiler
