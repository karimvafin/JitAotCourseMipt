#include <iostream>
#include <assert.h>

#include "RPO.hpp"

using namespace Compiler;

void test1(bool verbose = false) {
    Graph* graph = new Graph();

    /** Basic Blocks construction */
    BasicBlock* A = new BasicBlock(0, graph);
    BasicBlock* B = new BasicBlock(1, graph);
    BasicBlock* C = new BasicBlock(2, graph);
    BasicBlock* D = new BasicBlock(3, graph);
    BasicBlock* E = new BasicBlock(4, graph);
    BasicBlock* F = new BasicBlock(5, graph);
    BasicBlock* G = new BasicBlock(6, graph); 

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

    /** Add basic blocks to graph */
    graph->addBasicBlock({A, B, C, F, E, D, G});

    auto res = RPO(*graph);

    if (verbose) {
    for (const auto& elem : res) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    }

    assert((res == std::vector<size_t>{0, 1, 5, 6, 4, 2, 3}));

    delete graph;
    delete A; delete B; delete C; delete F; delete E; delete D; delete G;
}

int main() {
    test1(true);
}