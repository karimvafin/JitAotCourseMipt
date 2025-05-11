#include "Graph.hpp"
#include "Function.hpp"

namespace Compiler {

BasicBlock* Graph::createStartBlock() {
    startBlock_ = new BasicBlock(this);
    startBlock_->id_ = blockIndexer_.getIndex();
    id2Block_[startBlock_->id_] = startBlock_;
    blocks_.push_back(startBlock_);
    return startBlock_;
}

BasicBlock* Graph::createBasicBlock() {
    BasicBlock* block = new BasicBlock(this);
    block->id_ = blockIndexer_.getIndex();
    blocks_.push_back(block);
    id2Block_[block->id_] = block;
    return block;
}

void Graph::addBasicBlock(BasicBlock* block) {
    blocks_.push_back(block);
    id2Block_[block->id_] = block;
}

void Graph::removeBasicBlock(BasicBlock* block) {
    blocks_.erase(std::find(blocks_.begin(), blocks_.end(), block));
    id2Block_.erase(block->id_);
}

Instruction* Graph::createInst(InstType instType, OpCode opCode) {
    Instruction* inst = new Instruction(instIndexer_.getIndex(), instType, opCode);
    insts_.push_back(inst);
    id2Inst_[inst->id_] = inst;
    return inst;
}

Instruction* Graph::createInstConst(InstType instType, Value* value) {
    Instruction* inst = new Instruction(instIndexer_.getIndex(), instType, OpCode::CNST, value);
    insts_.push_back(inst);
    id2Inst_[inst->id_] = inst;
    return inst;
}

Instruction* Graph::createInstStaticCall(Function* func) {
    Instruction* inst = new Instruction(instIndexer_.getIndex(), func->retType_, func);
    insts_.push_back(inst);
    id2Inst_[inst->id_] = inst;
    return inst;
}

Instruction* Graph::copyInstruction(Instruction* inst) {
    Instruction* instCopy;
    if (inst->opCode_ == OpCode::CNST) {
        Value* newValue = createValueFromCopy(inst->value_);
        instCopy = createInstConst(inst->type_, newValue);
    } else if (inst->opCode_ == OpCode::CALL) {
        instCopy = createInstStaticCall(inst->callee_);
    } else {
        instCopy = createInst(inst->type_, inst->opCode_);
    }
    return instCopy;
}

void Graph::addInstruction(Instruction* inst) {
    insts_.push_back(inst);
    id2Inst_[inst->id_] = inst;
}

void Graph::removeInstruction(Instruction* inst) {
    insts_.erase(std::find(insts_.begin(), insts_.end(), inst));
    id2Inst_.erase(inst->id_);
}

Value* Graph::createValueFromCopy(Value* value) {
    Value* val = new Value(value);
    values_.push_back(val);
    return val;
}

void Graph::makeInstructionConst(Instruction* inst, Value* value) {
    inst->opCode_ = OpCode::CNST;
    inst->value_ = value;
}


Graph* Graph::copyCalleeGraph(const Graph* calleeGraph) {
    Graph* graphCopy = new Graph(blockIndexer_, instIndexer_);
    std::unordered_map<BasicBlock*, BasicBlock*> blocksMap;
    std::unordered_map<Instruction*, Instruction*> instMap;
    for (auto b: calleeGraph->blocks()) {
        BasicBlock* newBlock = graphCopy->createBasicBlock();
        blocksMap[b] = newBlock;
        for (auto i: b->insts_) {
            Instruction* newInst = graphCopy->copyInstruction(i);
            instMap[i] = newInst;
            newBlock->addInstructionToEnd(newInst);
        }
    }
    graphCopy->startBlock_ = graphCopy->blocks()[0];
    /// Add successors to new blocks
    for (auto [oldBlock, newBlock]: blocksMap) {
        if (oldBlock->firstSuccs_) {
            newBlock->addSuccessor(blocksMap[oldBlock->firstSuccs_], true);
        }
        if (oldBlock->secondSuccs_) {
            newBlock->addSuccessor(blocksMap[oldBlock->secondSuccs_], false);
        }
    }
    /// Add inputs to new instructions
    for (auto [oldInst, newInst]: instMap) {
        for (Instruction* i: oldInst->inputs_) {
            newInst->addInput(instMap[i]);
        }
    }

    return graphCopy;
}


Graph::~Graph() {
    for (auto& b : blocks_) {
        delete b;
    }
    
    for (auto& i : insts_) {
        delete i;
    }

    for (auto& v : values_) {
        delete v;
    }
}

}  // namespace Compiler
