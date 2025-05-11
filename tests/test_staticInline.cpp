#include "Graph.hpp"
#include "Function.hpp"
#include "optimizations/StaticInline.hpp"

using namespace Compiler;

void test_simple_inline(bool verbose = false) {
    /*
    BEFORE INLINE
    Caller graph
    ----------------------------
    Block 0
    Inst 1 CNST { } -> { 6 }
    Inst 2 CNST { } -> { 6 }

    Block 1
    Inst 3 CNST { } -> { 5 }
    Inst 4 CNST { } -> { 5 }
    Inst 5 CALL { 3 4 } -> { 6 }
    Inst 6 ADD { 5 1 2 } -> { }

    ---------------------------- 
    Callee graph
    ----------------------------
    Block 0
    Inst 7 PRM { } -> { 2 }
    Inst 8 PRM { } -> { 2 }

    Block 1
    Inst 9 ADD { 0 1 } -> { 3 }

    Block 2
    Inst 10 MOV { 2 } -> { 4 }
    Inst 11 RET { 3 } -> { }
    ----------------------------

    AFTER INLINE
    ----------------------------
    Block 0
    Inst 1 CNST { } -> { 6 }
    Inst 2 CNST { } -> { 6 }

    Block 1
    Inst 3 CNST { } -> { 5 9 }
    Inst 4 CNST { } -> { 5 9 }
    Inst 5 CALL { 3 4 } -> { }

    Block 5
    Inst 12 MOV { 10 } -> { 6 }
    Inst 6 ADD { 12 1 2 } -> { }

    Block 3
    Inst 9 ADD { 3 4 } -> { 10 }

    Block 4
    Inst 10 MOV { 9 } -> { 12 }
    ----------------------------
    */
    Indexer bi1, bi2, ii1(1), ii2;
    Graph* callerGraph = new Graph(bi1, ii1);
    Graph* calleeGraph = new Graph(bi2, ii2);

    /// Start Caller graph
    BasicBlock* b0 = callerGraph->createStartBlock();

    Value* val1 = callerGraph->createValue<int32_t>(1);
    Instruction* inst1 = callerGraph->createInstConst(InstType::S32, val1);

    Value* val5 = callerGraph->createValue<int32_t>(5);
    Instruction* inst2 = callerGraph->createInstConst(InstType::S32, val5);

    b0->addInstructionToEnd({inst1, inst2});

    BasicBlock* b1 = callerGraph->createBasicBlock();
    
    Value* val3 = callerGraph->createValue<int32_t>(3);
    Instruction* inst3 = callerGraph->createInstConst(InstType::S32, val3);
    Value* val4 = callerGraph->createValue<int32_t>(4);
    Instruction* inst4 = callerGraph->createInstConst(InstType::S32, val4);

    Function* func = createFunction(calleeGraph, "func", InstType::S32);

    Instruction* inst5 = callerGraph->createInstStaticCall(func);
    Instruction* inst6 = callerGraph->createInst(InstType::S32, OpCode::ADD);
    inst5->addInput({inst3, inst4});
    inst6->addInput({inst5, inst1, inst2});

    b1->addInstructionToEnd({inst3, inst4, inst5, inst6});
    if (verbose) {
        std::cout << "Caller graph before inlining" << std::endl;
        callerGraph->print();
    }

    b0->addSuccessor(b1);

    /// End Caller graph

    /// Start Callee graph
    BasicBlock* b2 = calleeGraph->createStartBlock();
    
    Instruction* inst11 = calleeGraph->createInst(InstType::S32, OpCode::PRM);
    Instruction* inst12 = calleeGraph->createInst(InstType::S32, OpCode::PRM);
    
    b2->addInstructionToEnd({inst11, inst12});
    
    BasicBlock* b3 = calleeGraph->createBasicBlock();
    
    Instruction* inst13 = calleeGraph->createInst(InstType::S32, OpCode::ADD);
    inst13->addInput({inst11, inst12});

    b3->addInstructionToEnd(inst13);
    
    BasicBlock* b4 = calleeGraph->createBasicBlock();

    Instruction* inst14 = calleeGraph->createInst(InstType::S32, OpCode::MOV);
    Instruction* inst15 = calleeGraph->createInst(InstType::S32, OpCode::RET);
    
    inst14->addInput(inst13);
    inst15->addInput(inst14);
    
    b4->addInstructionToEnd({inst14, inst15});

    if (verbose) {
        std::cout << "Callee graph before inlining" << std::endl;
        calleeGraph->print();
    }

    b2->addSuccessor(b3);
    b3->addSuccessor(b4);
    /// End Callee graph

    staticInline(callerGraph);
    
    /// Tests Data Flow
    assert(inst1->inputs_.empty());
    assert(inst2->inputs_.empty());
    assert(inst1->users_.size() == 1 && inst1->users_[0]->id_ == 6);
    assert(inst2->users_.size() == 1 && inst2->users_[0]->id_ == 6);

    assert(inst3->inputs_.empty());
    assert(inst4->inputs_.empty());
    assert(inst3->users_.size() == 2 && inst3->users_[0]->id_ == 5 && inst3->users_[1]->id_ == 9);
    assert(inst4->users_.size() == 2 && inst4->users_[0]->id_ == 5 && inst4->users_[1]->id_ == 9);
    assert(inst5->users_.empty());

    const auto& i12 = callerGraph->id2Inst().at(12);
    assert(i12->opCode_ == OpCode::MOV);
    assert(i12->inputs_.size() == 1 && i12->inputs_[0]->id_ == 10);
    assert(i12->users_.size() == 1 && i12->users_[0]->id_ == 6);

    const auto& i10 = callerGraph->id2Inst().at(10);
    assert(i10->opCode_ == OpCode::MOV);
    assert(i10->inputs_.size() == 1 && i10->inputs_[0]->id_ == 9);
    assert(i10->users_.size() == 1 && i10->users_[0]->id_ == 12);

    /// Tests Control Flow
    const auto& bb0 = callerGraph->id2Block().at(0);
    assert(bb0->firstSuccs_->id_ == 1);

    const auto& bb1 = callerGraph->id2Block().at(1);
    assert(bb1->firstSuccs_->id_ == 3);

    const auto& bb3 = callerGraph->id2Block().at(3);
    assert(bb3->firstSuccs_->id_ == 4);

    const auto& bb4 = callerGraph->id2Block().at(4);
    assert(bb4->firstSuccs_->id_ == 5);

    if (verbose) {
        std::cout << "After inlining" << std::endl;
        callerGraph->print();
    }

    delete callerGraph;
    delete calleeGraph; 
}

void test_from_lecture(bool verbose = false) {
    Indexer bi1, bi2, ii1(1), ii2;
    Graph* callerGraph = new Graph(bi1, ii1);
    Graph* calleeGraph = new Graph(bi2, ii2);

    /// Start Caller graph
    BasicBlock* b0 = callerGraph->createStartBlock();

    Value* val1 = callerGraph->createValue<int32_t>(1);
    Instruction* inst1 = callerGraph->createInstConst(InstType::S32, val1);

    Value* val5 = callerGraph->createValue<int32_t>(5);
    Instruction* inst2 = callerGraph->createInstConst(InstType::S32, val5);

    b0->addInstructionToEnd({inst1, inst2});

    BasicBlock* b1 = callerGraph->createBasicBlock();
    
    Value* val3 = callerGraph->createValue<int32_t>(3);
    Instruction* inst3 = callerGraph->createInstConst(InstType::S32, val3);
    Value* val4 = callerGraph->createValue<int32_t>(4);
    Instruction* inst4 = callerGraph->createInstConst(InstType::S32, val4);

    Function* func = createFunction(calleeGraph, "func", InstType::S32);

    Instruction* inst5 = callerGraph->createInstStaticCall(func);
    Instruction* inst6 = callerGraph->createInst(InstType::S32, OpCode::ADD);
    inst5->addInput({inst3, inst4});
    inst6->addInput({inst5, inst1, inst2});

    b1->addInstructionToEnd({inst3, inst4, inst5, inst6});

    if (verbose) {
        std::cout << "Caller graph before inlining" << std::endl;
        callerGraph->print();
    }

    b0->addSuccessor(b1);

    /// End Caller graph

    /// Start Callee graph
    BasicBlock* b2 = calleeGraph->createStartBlock();
    
    Instruction* inst11 = calleeGraph->createInst(InstType::S32, OpCode::PRM);
    Instruction* inst12 = calleeGraph->createInst(InstType::S32, OpCode::PRM);
    Instruction* inst19 = calleeGraph->createInstConst(InstType::S32, calleeGraph->createValue<int32_t>(1));
    Instruction* inst20 = calleeGraph->createInstConst(InstType::S32, calleeGraph->createValue<int32_t>(10));
    
    b2->addInstructionToEnd({inst11, inst12, inst19, inst20});
    
    BasicBlock* b3 = calleeGraph->createBasicBlock();
    
    Instruction* inst13 = calleeGraph->createInst(InstType::S32, OpCode::ADD);
    Instruction* inst14 = calleeGraph->createInst(InstType::S32, OpCode::ADD);
    inst13->addInput({inst11, inst19});
    inst14->addInput({inst12, inst20});

    b3->addInstructionToEnd({inst13, inst14});
    
    BasicBlock* b4 = calleeGraph->createBasicBlock();
    BasicBlock* b5 = calleeGraph->createBasicBlock();

    Instruction* inst15 = calleeGraph->createInst(InstType::S32, OpCode::MOV);
    Instruction* inst17 = calleeGraph->createInst(InstType::S32, OpCode::MOV);
    Instruction* inst16 = calleeGraph->createInst(InstType::S32, OpCode::RET);
    Instruction* inst18 = calleeGraph->createInst(InstType::S32, OpCode::RET);
    
    inst16->addInput(inst15);
    inst18->addInput(inst17);
    
    b4->addInstructionToEnd({inst15, inst16});
    b5->addInstructionToEnd({inst17, inst18});

    if (verbose) {
        std::cout << "Callee graph before inlining" << std::endl;
        callerGraph->print();
    }

    b2->addSuccessor(b3);
    b3->addSuccessor(b4, true);
    b3->addSuccessor(b5, false);
    /// End Callee graph

    staticInline(callerGraph);
    
    if (verbose) {
        std::cout << "After inlining" << std::endl;
        callerGraph->print();
    }

    /// Tests Data Flow
    const auto& i1 = callerGraph->id2Inst().at(1);
    const auto& i2 = callerGraph->id2Inst().at(2);
    const auto& i9 = callerGraph->id2Inst().at(9);
    const auto& i10 = callerGraph->id2Inst().at(10);
    const auto& i3 = callerGraph->id2Inst().at(3);
    const auto& i4 = callerGraph->id2Inst().at(4);
    const auto& i5 = callerGraph->id2Inst().at(5);
    const auto& i17 = callerGraph->id2Inst().at(17);
    const auto& i6 = callerGraph->id2Inst().at(6);
    const auto& i11 = callerGraph->id2Inst().at(11);
    const auto& i12 = callerGraph->id2Inst().at(12);
    const auto& i13 = callerGraph->id2Inst().at(13);
    const auto& i15 = callerGraph->id2Inst().at(15);

    const auto& bb0 = callerGraph->id2Block().at(0);
    const auto& bb1 = callerGraph->id2Block().at(1);
    const auto& bb6 = callerGraph->id2Block().at(6);
    const auto& bb3 = callerGraph->id2Block().at(3);
    const auto& bb4 = callerGraph->id2Block().at(4);
    const auto& bb5 = callerGraph->id2Block().at(5);

    assert(i1->inputs_.empty());
    assert(i2->inputs_.empty());
    assert(i1->users_.size() == 1 && i1->users_[0]->id_ == 6);
    assert(i2->users_.size() == 1 && i2->users_[0]->id_ == 6);
    assert(i9->inputs_.empty());
    assert(i10->inputs_.empty());
    assert(i9->users_.size() == 1 && i9->users_[0]->id_ == 11);
    assert(i10->users_.size() == 1 && i10->users_[0]->id_ == 12);

    assert(i3->inputs_.empty());
    assert(i4->inputs_.empty());
    assert(i3->users_.size() == 2 && i3->users_[0]->id_ == 5 && i3->users_[1]->id_ == 11);
    assert(i4->users_.size() == 2 && i4->users_[0]->id_ == 5 && i4->users_[1]->id_ == 12);
    assert(i5->users_.empty());

    assert(i17->opCode_ == OpCode::PHI);
    assert(i17->inputs_.size() == 2 && i17->inputs_[0]->id_ == 13 && i17->inputs_[1]->id_ == 15);
    assert(i17->users_.size() == 1 && i17->users_[0]->id_ == 6);

    assert(i6->inputs_.size() == 3 && i6->inputs_[0]->id_ == 17 && i6->inputs_[1]->id_ == 1 && i6->inputs_[2]->id_ == 2);

    assert(i11->inputs_.size() == 2 && i11->inputs_[0]->id_ == 3 && i11->inputs_[1]->id_ == 9);
    assert(i12->inputs_.size() == 2 && i12->inputs_[0]->id_ == 4 && i12->inputs_[1]->id_ == 10);

    assert(i13->users_.size() == 1 && i13->users_[0]->id_ == 17);
    assert(i15->users_.size() == 1 && i15->users_[0]->id_ == 17);

    /// Tests Control Flow
    assert(bb0->firstSuccs_->id_ == 1);
    assert(bb1->firstSuccs_->id_ == 3);
    assert(bb3->firstSuccs_->id_ == 4);
    assert(bb3->secondSuccs_->id_ == 5);
    assert(bb4->firstSuccs_->id_ == 6);
    assert(bb5->firstSuccs_->id_ == 6);

    if (verbose) {
        std::cout << "After inlining" << std::endl;
        callerGraph->print();
    }

    delete callerGraph;
    delete calleeGraph; 
}

int main() {
    test_simple_inline();
    test_from_lecture();
}
