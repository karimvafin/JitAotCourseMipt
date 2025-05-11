#include "assert.h"
#include <iostream>

#include "Graph.hpp"
#include "optimizations/ConstantFolding.hpp"

using namespace Compiler;

void testConstantFolding() {
    /** Graph construction */
    Indexer blockIndexer;
    Indexer instIndexer;
    Graph* graph = new Graph(blockIndexer, instIndexer);

    /** Basic Blocks construction */
    BasicBlock* bb0 = graph->createStartBlock(); // start

    /** Instructions construction */
    Instruction* inst00 = graph->createInst(InstType::U64, OpCode::CNST); 
    Instruction* inst01 = graph->createInst(InstType::U64, OpCode::CNST);
    Instruction* inst02 = graph->createInst(InstType::U64, OpCode::ADD);
    Instruction* inst03 = graph->createInst(InstType::U64, OpCode::SHR);
    Instruction* inst04 = graph->createInst(InstType::U64, OpCode::OR);
    Instruction* inst05 = graph->createInst(InstType::U64, OpCode::RET);

    /** Add inputs */
    inst02->addInput({inst00, inst01});
    inst03->addInput({inst00, inst02});
    inst04->addInput({inst03, inst01});
    inst05->addInput(inst04);

    /** Add instructions to basic blocks */
    bb0->addInstructionToEnd({inst00, inst01, inst02, inst03, inst04, inst05});

    /** Add predecessors and successors to basic blocks */

    /** TESTS */

    assert((graph->blocks()[0]->getInstructionIds() == std::vector<size_t>{0, 1, 2, 3, 4, 5}));

    constantFolding(graph);

    assert((graph->blocks()[0]->getInstructionIds() == std::vector<size_t>{4, 5}));
    
    delete graph;
}

int main() {
    testConstantFolding();
}
