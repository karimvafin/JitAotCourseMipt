#include "assert.h"

#include "Graph.hpp"

using namespace Compiler;

void testGraphFactorial() {
    /** Graph construction */
    Graph* graph = new Graph();

    /** Basic Blocks construction */
    BasicBlock* bb0 = new BasicBlock(0, graph); // start
    BasicBlock* bb1 = new BasicBlock(1, graph); // cmp
    BasicBlock* bb2 = new BasicBlock(2, graph); // mult
    BasicBlock* bb3 = new BasicBlock(3, graph); // ret

    /** Instructions construction */
    Instruction* inst0  = new Instruction(11, InstType::S32, OpCode::PRM);
    Instruction* inst00 = new Instruction(0, InstType::U64, OpCode::MOV);  // res1 = 1
    Instruction* inst01 = new Instruction(1, InstType::U64, OpCode::MOV);  // i1 = 2
    Instruction* inst02 = new Instruction(2, InstType::U64, OpCode::CAST); // (u64)n1

    Instruction* inst10 = new Instruction(3, InstType::U32, OpCode::PHI);  // i3 = phi(i1, i2)
    Instruction* inst11 = new Instruction(4, InstType::B, OpCode::CMP);    // b0 = cmp(i3, n1)
    Instruction* inst12 = new Instruction(5, InstType::VOID, OpCode::JMPC); 

    Instruction* inst20 = new Instruction(6, InstType::U64, OpCode::PHI);  // res3 = phi(res1, res2)
    Instruction* inst21 = new Instruction(7, InstType::U64, OpCode::MUL);  // res2 = res3 * i3
    Instruction* inst22 = new Instruction(8, InstType::U32, OpCode::ADD);  // i2 = i3 + 1
    Instruction* inst23 = new Instruction(9, InstType::VOID, OpCode::JMP);

    Instruction* inst30 = new Instruction(10, InstType::U64, OpCode::RET); // return res3

    /** Add inputs */
    inst02->addInput(inst0);
    inst10->addInput({inst01, inst22});
    inst11->addInput({inst10, inst02});
    inst20->addInput({inst00, inst21});
    inst21->addInput({inst20, inst10});
    inst22->addInput(inst10);
    inst30->addInput(inst20);

    /** Add instructions to basic blocks */
    bb0->addInstructionToEnd({inst0, inst00, inst01, inst02});
    bb1->addInstructionToEnd({inst10, inst11, inst12});
    bb2->addInstructionToEnd({inst20, inst21, inst22, inst23});
    bb3->addInstructionToEnd(inst30);

    /** Add predecessors and successors to basic blocks */
    bb0->addSuccessor(bb1, true);

    bb1->addSuccessor(bb2, true);
    bb1->addSuccessor(bb3, false);

    bb2->addSuccessor(bb1, true);

    /** Add basic blocks to graph */
    graph->addBasicBlock({bb0, bb1, bb2, bb3});

    /** TESTS */
    assert((bb0->getInstructionIds() == std::vector<size_t>{11, 0, 1, 2}));
    assert((bb1->getInstructionIds() == std::vector<size_t>{3, 4, 5}));
    assert((bb2->getInstructionIds() == std::vector<size_t>{6, 7, 8, 9}));
    assert((bb3->getInstructionIds() == std::vector<size_t>{10}));

    assert((bb0->preds_ == std::vector<BasicBlock*>{}));
    assert((bb0->firstSuccs_ == bb1));
    assert((bb1->preds_ == std::vector<BasicBlock*>{bb0, bb2}));
    assert((bb1->firstSuccs_ == bb2));
    assert((bb1->secondSuccs_ == bb3));
    assert((bb2->preds_ == std::vector<BasicBlock*>{bb1}));
    assert((bb2->firstSuccs_ == bb1));
    assert((bb3->preds_ == std::vector<BasicBlock*>{bb1}));
    assert((bb3->firstSuccs_ == nullptr));
    
    delete graph;
    delete bb0; delete bb1; delete bb2; delete bb3;
    delete inst0;  delete inst00; delete inst01; delete inst02; delete inst10; delete inst11; 
    delete inst12; delete inst20; delete inst21; delete inst22; delete inst23; delete inst30;
}

int main() {
    testGraphFactorial();
}