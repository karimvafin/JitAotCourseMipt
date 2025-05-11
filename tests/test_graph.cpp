#include "assert.h"

#include "Graph.hpp"

using namespace Compiler;

void testGraphFactorial() {
    /** Graph construction */
    Indexer blockIndexer;
    Indexer instIndexer;
    Graph* graph = new Graph(blockIndexer, instIndexer);

    /** Basic Blocks construction */
    BasicBlock* bb0 = graph->createStartBlock(); // start
    BasicBlock* bb1 = graph->createBasicBlock(); // cmp
    BasicBlock* bb2 = graph->createBasicBlock(); // mult
    BasicBlock* bb3 = graph->createBasicBlock(); // ret

    /** Instructions construction */
    Instruction* inst0  = graph->createInst(InstType::S32, OpCode::PRM);
    Instruction* inst00 = graph->createInst(InstType::U64, OpCode::MOV);  // res1 = 1
    Instruction* inst01 = graph->createInst(InstType::U64, OpCode::MOV);  // i1 = 2
    Instruction* inst02 = graph->createInst(InstType::U64, OpCode::CAST); // (u64)n1

    Instruction* inst10 = graph->createInst(InstType::U32, OpCode::PHI);  // i3 = phi(i1, i2)
    Instruction* inst11 = graph->createInst(InstType::B, OpCode::CMP);    // b0 = cmp(i3, n1)
    Instruction* inst12 = graph->createInst(InstType::VOID, OpCode::JMPC); 

    Instruction* inst20 = graph->createInst(InstType::U64, OpCode::PHI);  // res3 = phi(res1, res2)
    Instruction* inst21 = graph->createInst(InstType::U64, OpCode::MUL);  // res2 = res3 * i3
    Instruction* inst22 = graph->createInst(InstType::U32, OpCode::ADD);  // i2 = i3 + 1
    Instruction* inst23 = graph->createInst(InstType::VOID, OpCode::JMP);

    Instruction* inst30 = graph->createInst(InstType::U64, OpCode::RET); // return res3

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

    /** TESTS */
    assert((bb0->getInstructionIds() == std::vector<size_t>{0, 1, 2, 3}));
    assert((bb1->getInstructionIds() == std::vector<size_t>{4, 5, 6}));
    assert((bb2->getInstructionIds() == std::vector<size_t>{7, 8, 9, 10}));
    assert((bb3->getInstructionIds() == std::vector<size_t>{11}));

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
}

int main() {
    testGraphFactorial();
}
