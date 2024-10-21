#ifndef DOMTREE_HPP
#define DOMTREE_HPP

#include <vector>
#include "Graph.hpp"

namespace Compiler {

struct DomTree {
    std::vector<std::vector<size_t>> tree_;

    static DomTree createDomTree(const Graph& graph);

    static std::vector<size_t> dfs(const Graph& graph);

    static std::vector<size_t> dfsWithExcluded(const Graph& graph, int excluded);

    static std::vector<size_t> calcRemainingBlocks(const std::vector<size_t>& dfsResult, size_t numBlocks);

    static std::vector<std::vector<size_t>> createReverseGraph(const std::vector<std::vector<size_t>>& graph);

    static std::vector<std::vector<size_t>> calcDominated(const Graph& graph);

    static std::vector<std::vector<size_t>> calcDominators(const std::vector<std::vector<size_t>>& dominated);

    static std::vector<std::vector<size_t>> checkForImmediate();
};

}  // namespace Compiler

#endif  // DOMTREE_HPP