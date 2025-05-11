#include "assert.h"
#include <iostream>

#include "Graph.hpp"
#include "optimizations/Peephole.hpp"

using namespace Compiler;

void testPeepholes() {
    /** Graph construction */
    Indexer bi, ii;
    Graph* graph = new Graph(bi, ii);

    /** Basic Blocks construction */
    BasicBlock* bb0 = graph->createStartBlock(); // start

    /** Instructions construction */
    Value* val0 = graph->createValue<uint32_t>(0);
    Value* val100 = graph->createValue<uint32_t>(100);

    Instruction* inst00 = graph->createInstConst(InstType::U64, val0);
    Instruction* inst01 = graph->createInstConst(InstType::U64, val100);

    Instruction* inst02 = graph->createInst(InstType::U64, OpCode::ADD);
    Instruction* inst03 = graph->createInst(InstType::U64, OpCode::ADD);

    Instruction* inst04 = graph->createInst(InstType::U64, OpCode::SHR);
    Instruction* inst05 = graph->createInst(InstType::U64, OpCode::SHR);

    Instruction* inst06 = graph->createInst(InstType::U64, OpCode::OR);
    Instruction* inst07 = graph->createInst(InstType::U64, OpCode::OR);

    Instruction* inst08 = graph->createInst(InstType::U64, OpCode::RET);

    /** Add inputs */
    inst02->addInput({inst00, inst01});
    inst03->addInput({inst01, inst00});

    inst04->addInput({inst02, inst00});
    inst05->addInput({inst02, inst01});

    inst06->addInput({inst01, inst00});
    inst07->addInput({inst01, inst01});

    inst08->addInput(inst07);

    /** Add instructions to basic blocks */
    bb0->addInstructionToEnd({inst00, inst01, inst02, inst03, inst04, inst05, inst06, inst07, inst08});

    /** Add predecessors and successors to basic blocks */

    /** TESTS */
    assert(graph->blocks()[0]->insts_[0]->opCode_ == OpCode::CNST);
    assert(graph->blocks()[0]->insts_[1]->opCode_ == OpCode::CNST);
    assert(graph->blocks()[0]->insts_[2]->opCode_ == OpCode::ADD);
    assert(graph->blocks()[0]->insts_[3]->opCode_ == OpCode::ADD);
    assert(graph->blocks()[0]->insts_[4]->opCode_ == OpCode::SHR);
    assert(graph->blocks()[0]->insts_[5]->opCode_ == OpCode::SHR);
    assert(graph->blocks()[0]->insts_[6]->opCode_ == OpCode::OR);
    assert(graph->blocks()[0]->insts_[7]->opCode_ == OpCode::OR);
    assert(graph->blocks()[0]->insts_[8]->opCode_ == OpCode::RET);

    peepholeOptimization(graph);

    // #1 Add zero
    assert(inst02->opCode_ == OpCode::MOV);

    // #2 Communicative add
    assert(inst03->opCode_ == OpCode::MOV);

    // #1 Shift to zero
    assert(inst04->opCode_ == OpCode::MOV);

    // #2 Shift to large number
    assert(inst05->opCode_ == OpCode::CNST);

    // #1 Or with zero
    assert(inst06->opCode_ == OpCode::MOV);

    // #2 Or with self
    assert(inst07->opCode_ == OpCode::CNST);
    
    delete graph;
}

int main() {
    testPeepholes();
}
