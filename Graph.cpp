#include "Graph.hpp"

void Graph::addBasicBlock(BasicBlock* block) {
    blocks_.push_back(block);
}


void Graph::addBasicBlock(const std::vector<BasicBlock*>& blocks) {
    for (const auto block : blocks) {
        blocks_.push_back(block);
    }
}