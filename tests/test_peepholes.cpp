#include "assert.h"
#include <iostream>

#include "Graph.hpp"
#include "Optimizations.hpp"

using namespace Compiler;

void testPeepholes() {
    /** Graph construction */
    Graph* graph = new Graph();

    /** Basic Blocks construction */
    BasicBlock* bb0 = new BasicBlock(0, graph); // start

    /** Instructions construction */
    Instruction* inst00 = new Instruction(0, InstType::U64, OpCode::CNST, Value{0});
    Instruction* inst01 = new Instruction(1, InstType::U64, OpCode::CNST, Value{100});

    Instruction* inst02 = new Instruction(2, InstType::U64, OpCode::ADD);
    Instruction* inst03 = new Instruction(3, InstType::U64, OpCode::ADD);

    Instruction* inst04 = new Instruction(4, InstType::U64, OpCode::SHR);
    Instruction* inst05 = new Instruction(5, InstType::U64, OpCode::SHR);

    Instruction* inst06 = new Instruction(6, InstType::U64, OpCode::OR);
    Instruction* inst07 = new Instruction(7, InstType::U64, OpCode::OR);

    Instruction* inst08 = new Instruction(8, InstType::U64, OpCode::RET);

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

    /** Add basic blocks to graph */
    graph->addBasicBlock(bb0);

    /** TESTS */
    assert(graph->blocks_[0]->insts_[0]->opCode_ == OpCode::CNST);
    assert(graph->blocks_[0]->insts_[1]->opCode_ == OpCode::CNST);
    assert(graph->blocks_[0]->insts_[2]->opCode_ == OpCode::ADD);
    assert(graph->blocks_[0]->insts_[3]->opCode_ == OpCode::ADD);
    assert(graph->blocks_[0]->insts_[4]->opCode_ == OpCode::SHR);
    assert(graph->blocks_[0]->insts_[5]->opCode_ == OpCode::SHR);
    assert(graph->blocks_[0]->insts_[6]->opCode_ == OpCode::OR);
    assert(graph->blocks_[0]->insts_[7]->opCode_ == OpCode::OR);
    assert(graph->blocks_[0]->insts_[8]->opCode_ == OpCode::RET);

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
    assert(inst06->opCode_ == OpCode::CNST);

    // #2 Or with self
    assert(inst07->opCode_ == OpCode::CNST);
    
    delete graph;
    delete bb0;
    delete inst00; delete inst01; delete inst02;
    delete inst03; delete inst04; delete inst05;
    delete inst06; delete inst07; delete inst08;
}

int main() {
    testPeepholes();
}