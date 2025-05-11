#include <iostream>
#include <assert.h>

#include "RPO.hpp"

using namespace Compiler;

void test1(bool verbose = false) {
    Indexer blockIndexer;
    Indexer instIndexer;
    Graph* graph = new Graph(blockIndexer, instIndexer);

    /** Basic Blocks construction */
    BasicBlock* A = graph->createStartBlock();
    BasicBlock* B = graph->createBasicBlock();
    BasicBlock* C = graph->createBasicBlock();
    BasicBlock* D = graph->createBasicBlock();
    BasicBlock* E = graph->createBasicBlock();
    BasicBlock* F = graph->createBasicBlock();
    BasicBlock* G = graph->createBasicBlock(); 

    /** Add predecessors and successors to basic blocks */
    A->addSuccessor(B, true);

    B->addSuccessor(C, true);
    B->addSuccessor(F, false);

    C->addSuccessor(D, true);

    F->addSuccessor(E, true);
    F->addSuccessor(G, false);

    E->addSuccessor(D, true);

    G->addSuccessor(D, true);
    E->addSuccessor(B, false);

    auto res = RPO(*graph);

    if (verbose) {
    for (const auto& elem : res) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    }

    assert((res == std::vector<size_t>{0, 1, 5, 6, 4, 2, 3}));

    delete graph;
}

int main() {
    test1(true);
}
