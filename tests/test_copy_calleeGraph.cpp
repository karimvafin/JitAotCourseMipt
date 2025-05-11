#include "Graph.hpp"
#include "Function.hpp"
#include "optimizations/StaticInline.hpp"

using namespace Compiler;

int main() {
    Indexer blockIndexer;
    Indexer instIndexer;
    Graph* callerGraph = new Graph(blockIndexer, instIndexer);
    Graph* calleeGraph = new Graph(blockIndexer, instIndexer);

    /// Start Callee graph
    BasicBlock* b2 = calleeGraph->createStartBlock();
    
    Instruction* inst20 = calleeGraph->createInst(InstType::S32, OpCode::PRM);
    Instruction* inst21 = calleeGraph->createInst(InstType::S32, OpCode::PRM);

    Value* val22 = calleeGraph->createValue<int32_t>(1);
    Instruction* inst22 = calleeGraph->createInstConst(InstType::S32, val22);

    Value* val23 = calleeGraph->createValue<int32_t>(10);
    Instruction* inst23 = calleeGraph->createInstConst(InstType::S32, val23);
    
    b2->addInstructionToEnd({inst20, inst21, inst22, inst23});
    
    BasicBlock* b3 = calleeGraph->createBasicBlock();
    
    Instruction* inst30 = calleeGraph->createInst(InstType::S32, OpCode::ADD);
    Instruction* inst31 = calleeGraph->createInst(InstType::S32, OpCode::ADD);
    inst30->addInput({inst20, inst22});
    inst31->addInput({inst21, inst23});

    b3->addInstructionToEnd({inst30, inst31});
    
    BasicBlock* b4 = calleeGraph->createBasicBlock();
    BasicBlock* b5 = calleeGraph->createBasicBlock();

    Instruction* inst40 = calleeGraph->createInst(InstType::S32, OpCode::RET);
    Instruction* inst41 = calleeGraph->createInst(InstType::S32, OpCode::RET);
    
    inst40->addInput(inst30);
    inst41->addInput(inst31);
    
    b4->addInstructionToEnd(inst40);
    b5->addInstructionToEnd(inst41);
    /// End Callee graph

    /// Start Caller graph
    BasicBlock* b0 = callerGraph->createStartBlock();

    Value* val00 = callerGraph->createValue<int32_t>(1);
    Instruction* inst00 = callerGraph->createInstConst(InstType::S32, val00);

    Value* val01 = callerGraph->createValue<int32_t>(5);
    Instruction* inst01 = callerGraph->createInstConst(InstType::S32, val01);

    b0->addInstructionToEnd({inst00, inst01});

    BasicBlock* b1 = callerGraph->createBasicBlock();
    
    Value* val10 = callerGraph->createValue<int32_t>(0);
    Instruction* inst10 = callerGraph->createInstConst(InstType::S32, val10);
    Value* val11 = callerGraph->createValue<int32_t>(0);
    Instruction* inst11 = callerGraph->createInstConst(InstType::S32, val11);

    Function* func = createFunction(calleeGraph, "func", InstType::S32);

    Instruction* inst12 = callerGraph->createInstStaticCall(func);
    Instruction* inst13 = callerGraph->createInst(InstType::S32, OpCode::ADD);
    Instruction* inst14 = callerGraph->createInst(InstType::S32, OpCode::ADD);
    inst12->addInput({inst10, inst11});
    inst13->addInput({inst12, inst00});
    inst14->addInput({inst13, inst01});

    b1->addInstructionToEnd({inst10, inst11, inst12, inst13, inst14});
    /// End Caller graph

    Graph* copyCalleeGraph = calleeGraph->copyCalleeGraph(calleeGraph);
    
    // calleeGraph->print();
    // copyCalleeGraph->print();
    // calleeGraph->blocks().clear();
    
    assert(calleeGraph->blocks().size() == copyCalleeGraph->blocks().size());
    for (size_t i = 0; i < calleeGraph->blocks().size(); ++i) {
        auto calleeInsts = calleeGraph->blocks()[i]->insts_;
        auto copyCalleeInsts = copyCalleeGraph->blocks()[i]->insts_;
        assert(calleeInsts.size() == copyCalleeInsts.size());
        for (size_t j = 0; j < calleeInsts.size(); ++j) {
            assert(calleeInsts[j]->inputs_.size() == copyCalleeInsts[j]->inputs_.size());
            assert(calleeInsts[j]->users_.size() == copyCalleeInsts[j]->users_.size());
        }
    }

    delete callerGraph;
    delete calleeGraph; 
    delete copyCalleeGraph;
}
