#include <iostream>
#include <assert.h>

#include "DomTree.hpp"

using namespace Compiler;

void test1(bool verbose = false) {
    Graph* graph = new Graph();

    /** Basic Blocks construction */
    BasicBlock* A = new BasicBlock(0, graph);
    BasicBlock* B = new BasicBlock(1, graph);
    BasicBlock* C = new BasicBlock(2, graph);
    BasicBlock* F = new BasicBlock(3, graph);
    BasicBlock* E = new BasicBlock(4, graph);
    BasicBlock* D = new BasicBlock(5, graph);
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

    /** Add basic blocks to graph */
    graph->addBasicBlock({A, B, C, F, E, D, G});

    auto res = DomTree::createDomTree(*graph);

    if (verbose)
    for (const auto& row : res.tree_) {
        for (size_t element : row) {
        std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    assert((res.tree_[0] == std::vector<size_t>{1}));
    assert((res.tree_[1] == std::vector<size_t>{2, 3, 5}));
    assert((res.tree_[2] == std::vector<size_t>{}));
    assert((res.tree_[3] == std::vector<size_t>{4, 6}));
    assert((res.tree_[4] == std::vector<size_t>{}));
    assert((res.tree_[5] == std::vector<size_t>{}));
    assert((res.tree_[6] == std::vector<size_t>{}));

    delete graph;
    delete A; delete B; delete C; delete F; delete E; delete D; delete G;
}

void test2(bool verbose = false) {
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
    BasicBlock* J = new BasicBlock(9, graph); 
    BasicBlock* K = new BasicBlock(10, graph);

    /** Add predecessors and successors to basic blocks */
    A->addSuccessor(B, true);

    B->addSuccessor(C, true);
    B->addSuccessor(J, false);

    C->addSuccessor(D, true);

    D->addSuccessor(C, true);
    D->addSuccessor(E, false);

    E->addSuccessor(F, true);

    F->addSuccessor(E, true);
    F->addSuccessor(G, false);

    G->addSuccessor(H, true);
    G->addSuccessor(I, false);

    H->addSuccessor(B, true);

    I->addSuccessor(K, true);

    J->addSuccessor(C, true);

    /** Add basic blocks to graph */
    graph->addBasicBlock({A, B, C, D, E, F, G, H, I, J, K});

    auto res = DomTree::createDomTree(*graph);

    if (verbose)
    for (const auto& row : res.tree_) {
        for (size_t element : row) {
        std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    assert((res.tree_[0] == std::vector<size_t>{1}));
    assert((res.tree_[1] == std::vector<size_t>{2, 9}));
    assert((res.tree_[2] == std::vector<size_t>{3}));
    assert((res.tree_[3] == std::vector<size_t>{4}));
    assert((res.tree_[4] == std::vector<size_t>{5}));
    assert((res.tree_[5] == std::vector<size_t>{6}));
    assert((res.tree_[6] == std::vector<size_t>{7, 8}));
    assert((res.tree_[7] == std::vector<size_t>{}));
    assert((res.tree_[8] == std::vector<size_t>{10}));
    assert((res.tree_[9] == std::vector<size_t>{}));
    assert((res.tree_[10] == std::vector<size_t>{}));

    delete graph;
    delete A; delete B; delete C; delete D; delete E; delete F; delete G; delete H; delete I; delete J; delete K;
}

void test3(bool verbose = false) {
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
    B->addSuccessor(E, false);

    C->addSuccessor(D, true);

    D->addSuccessor(G, true);

    E->addSuccessor(F, true);
    E->addSuccessor(D, false);

    F->addSuccessor(B, true);
    F->addSuccessor(H, false);

    G->addSuccessor(C, true);
    G->addSuccessor(I, false);

    H->addSuccessor(G, true);
    H->addSuccessor(I, false);

    /** Add basic blocks to graph */
    graph->addBasicBlock({A, B, C, D, E, F, G, H, I});

    auto res = DomTree::createDomTree(*graph);

    if (verbose)
    for (const auto& row : res.tree_) {
        for (size_t element : row) {
        std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    assert((res.tree_[0] == std::vector<size_t>{1}));
    assert((res.tree_[1] == std::vector<size_t>{2, 3, 4, 6, 8}));
    assert((res.tree_[2] == std::vector<size_t>{}));
    assert((res.tree_[3] == std::vector<size_t>{}));
    assert((res.tree_[4] == std::vector<size_t>{5}));
    assert((res.tree_[5] == std::vector<size_t>{7}));
    assert((res.tree_[6] == std::vector<size_t>{}));
    assert((res.tree_[7] == std::vector<size_t>{}));
    assert((res.tree_[8] == std::vector<size_t>{}));

    delete graph;
    delete A; delete B; delete C; delete D; delete E; delete F; delete G; delete H; delete I;
}

int main() {
    test1();
    test2();
    test3();
}