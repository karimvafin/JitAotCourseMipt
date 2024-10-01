#include "assert.h"

#include "Graph.hpp"

void testGraphFactorial() {
    /** Graph construction */
    Graph* graph = new Graph();

    /** Basic Blocks construction */
    BasicBlock* bb0 = new BasicBlock(graph); // start
    BasicBlock* bb1 = new BasicBlock(graph); // cmp
    BasicBlock* bb2 = new BasicBlock(graph); // mult
    BasicBlock* bb3 = new BasicBlock(graph); // ret

    /** Instructions construction */
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
    inst10->addInput({1, 8});
    inst11->addInput({3, 2});
    inst20->addInput({0, 7});
    inst21->addInput({6, 3});
    inst22->addInput(3);
    inst30->addInput(6);

    /** Add users */
    inst00->addUser(6);
    inst01->addUser(3);
    inst02->addUser(4);
    inst10->addUser({4, 7, 8});
    inst20->addUser({7, 10});
    inst21->addUser(6);
    inst22->addUser(3);

    /** Add instructions to basic blocks */
    bb0->addInstruction({inst00, inst01, inst02});
    bb1->addInstruction({inst10, inst11, inst12});
    bb2->addInstruction({inst20, inst21, inst22, inst23});
    bb3->addInstruction(inst30);

    /** Add predecessors and successors to basic blocks */
    bb0->addSuccessor(bb1);

    bb1->addPredecessor(bb0);
    bb1->addSuccessor(bb2);
    bb1->addSuccessor(bb3);

    bb2->addPredecessor(bb1);
    bb2->addSuccessor(bb1);

    bb3->addPredecessor(bb1);

    /** Add basic blocks to graph */
    graph->addBasicBlock({bb0, bb1, bb2, bb3});

    /** TESTS */
    assert((bb0->getInstructionIds() == std::vector<size_t>{0, 1, 2}));
    assert((bb1->getInstructionIds() == std::vector<size_t>{3, 4, 5}));
    assert((bb2->getInstructionIds() == std::vector<size_t>{6, 7, 8, 9}));
    assert((bb3->getInstructionIds() == std::vector<size_t>{10}));

    assert((bb0->preds_ == std::vector<BasicBlock*>{}));
    assert((bb0->succs_ == std::vector<BasicBlock*>{bb1}));
    assert((bb1->preds_ == std::vector<BasicBlock*>{bb0}));
    assert((bb1->succs_ == std::vector<BasicBlock*>{bb2, bb3}));
    assert((bb2->preds_ == std::vector<BasicBlock*>{bb1}));
    assert((bb2->succs_ == std::vector<BasicBlock*>{bb1}));
    assert((bb3->preds_ == std::vector<BasicBlock*>{bb1}));
    assert((bb3->succs_ == std::vector<BasicBlock*>{}));
    
    delete graph;
    delete bb0; delete bb1; delete bb2; delete bb3;
    delete inst00; delete inst01; delete inst02; delete inst10; delete inst11; delete inst12;
    delete inst20; delete inst21; delete inst22; delete inst23; delete inst30;
}

int main() {
    testGraphFactorial();
}