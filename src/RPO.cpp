#include <assert.h>
#include <array>

#include "RPO.hpp"

namespace Compiler {

std::vector<size_t> RPO(const Graph& graph) {
    size_t blocksCount = graph.size();
    std::vector<size_t> rpoVector(blocksCount);
    std::vector<bool> vis(blocksCount, false);

    RPOImpl(graph.startBlock(), blocksCount, rpoVector, vis);

    return rpoVector;
}

void RPOImpl(const BasicBlock* block, size_t& blocksCount, std::vector<size_t>& rpoVector, std::vector<bool>& vis) {
    assert(block != nullptr);
    vis[block->id_] = true;

    std::array<BasicBlock*, 2> succs = {block->firstSuccs_, block->secondSuccs_};
    for (const auto succ : succs) {
        if (succ && !vis[succ->id_]) {
            RPOImpl(succ, blocksCount, rpoVector, vis);
        }
    }

    assert(blocksCount > 0);
    rpoVector[--blocksCount] = block->id_;
}

}  // namespace Compiler