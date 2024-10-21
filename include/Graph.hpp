#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "BasicBlock.hpp"

namespace Compiler {

struct Graph {
    std::vector<BasicBlock*> blocks_;

    void addBasicBlock(BasicBlock* block);
    void addBasicBlock(const std::vector<BasicBlock*>& blocks);

    size_t size() const {
        return blocks_.size();
    }
};

}  // namespace Compiler

#endif  // GRAPH_HPP
