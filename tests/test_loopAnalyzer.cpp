#include <iostream>
#include <assert.h>

#include "LoopAnalyzer.hpp"

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
    BasicBlock* H = new BasicBlock(7, graph);
    BasicBlock* I = new BasicBlock(8, graph);

    /** Add predecessors and successors to basic blocks */
    A->addSuccessor(B, true);

    B->addSuccessor(C, true);

    C->addSuccessor(D, true);
    C->addSuccessor(E, false);

    D->addSuccessor(B, true);

    E->addSuccessor(B, true);
    E->addSuccessor(F, false);

    F->addSuccessor(G, true);

    G->addSuccessor(H, true);
    G->addSuccessor(I, false);

    H->addSuccessor(F, true);

    /** Add basic blocks to graph */
    graph->addBasicBlock({A, B, C, D, E, F, G, H, I});

    LoopAnalyzer loopAnalyzer(graph);
    loopAnalyzer.findBackEdges();
    loopAnalyzer.populateLoops();
    loopAnalyzer.createRootLoop();

    if (verbose) {
        for (const auto& elem : loopAnalyzer.loops_) {
            std::cout << "Header = " << elem.header_ << "; ";
            std::cout << "Id = " << elem.id_ << "; ";
            std::cout << "BackEdges = ";
            for (auto i : elem.backEdges_) {
                std::cout << i << " ";
            }
            std::cout << "; ";
            std::cout << "Blocks: ";
            for (auto i : elem.blocks_) {
                std::cout << i << " ";
            }
            std::cout << "; ";
            std::cout << "Included loops: ";
            for (auto i : elem.otherLoops_) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
    }

    // assert((res == std::vector<size_t>{0, 1, 5, 6, 4, 2, 3}));

    delete graph;
    delete A; delete B; delete C; delete D; delete E; delete F; delete G; delete H; delete I;
}

int main() {
    test1(true);
}