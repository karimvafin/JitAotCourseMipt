#include "assert.h"
#include <iostream>

#include "Graph.hpp"
#include "Optimizations.hpp"

using namespace Compiler;

void testConstantFolding() {
    /** Graph construction */
    Graph* graph = new Graph();

    /** Basic Blocks construction */
    BasicBlock* bb0 = new BasicBlock(0, graph); // start

    /** Instructions construction */
    Instruction* inst00 = new Instruction(0, InstType::U64, OpCode::CNST); 
    Instruction* inst01 = new Instruction(1, InstType::U64, OpCode::CNST);
    Instruction* inst02 = new Instruction(2, InstType::U64, OpCode::ADD);
    Instruction* inst03 = new Instruction(3, InstType::U64, OpCode::SHR);
    Instruction* inst04 = new Instruction(4, InstType::U64, OpCode::OR);
    Instruction* inst05 = new Instruction(5, InstType::U64, OpCode::RET);

    /** Add inputs */
    inst02->addInput({inst00, inst01});
    inst03->addInput({inst00, inst02});
    inst04->addInput({inst03, inst01});
    inst05->addInput(inst04);

    /** Add instructions to basic blocks */
    bb0->addInstructionToEnd({inst00, inst01, inst02, inst03, inst04, inst05});

    /** Add predecessors and successors to basic blocks */

    /** Add basic blocks to graph */
    graph->addBasicBlock(bb0);

    /** TESTS */

    assert((graph->blocks_[0]->getInstructionIds() == std::vector<size_t>{0, 1, 2, 3, 4, 5}));

    constantFolding(graph);

    assert((graph->blocks_[0]->getInstructionIds() == std::vector<size_t>{4, 5}));
    
    delete graph;
    delete bb0;
    delete inst00; delete inst01; delete inst02; delete inst03; delete inst04;
}

int main() {
    testConstantFolding();
}