#include <iostream>
#include <assert.h>

#include "LoopAnalyzer.hpp"

using namespace Compiler;

void test1(bool verbose = false) {
    Indexer bi;     
    Indexer ii;
    Graph* graph = new Graph(bi, ii);

    /** Basic Blocks construction */
    BasicBlock* A = graph->createStartBlock();
    BasicBlock* B = graph->createBasicBlock();
    BasicBlock* C = graph->createBasicBlock();
    BasicBlock* D = graph->createBasicBlock();
    BasicBlock* E = graph->createBasicBlock();
    BasicBlock* F = graph->createBasicBlock();
    BasicBlock* G = graph->createBasicBlock();
    BasicBlock* H = graph->createBasicBlock();
    BasicBlock* I = graph->createBasicBlock();

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
}

void test2(bool verbose = false) {
    Indexer bi, ii;
    Graph* graph = new Graph(bi, ii);

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
    // E->addSuccessor(B, false);

    G->addSuccessor(D, true);

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
}

int main() {
    test1(false);
    test2(true);
}
