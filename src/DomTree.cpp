#include <stack>
#include <iostream>

#include "DomTree.hpp"
#include "assert.h"

namespace Compiler {

DomTree DomTree::createDomTree(const Graph& graph) {
    DomTree domTree;
    domTree.tree_.resize(graph.size());
    std::vector<std::vector<size_t>> dominated = calcDominated(graph);
    std::vector<std::vector<size_t>> dominators = calcDominators(dominated);
    for (size_t vertex = 0; vertex < dominators.size(); ++vertex) {
        for (size_t dom : dominators[vertex]) {
            bool isIdom = true;
            for (size_t otherDom : dominators[vertex]) {
                if (otherDom == dom) continue;
                if (std::find(dominated[otherDom].begin(), dominated[otherDom].end(), dom) == dominated[otherDom].end()) {
                    isIdom = false; 
                    break;
                }
            }
            if (isIdom) domTree.tree_[dom].push_back(vertex);
        }
    }
    return domTree;
}

std::vector<size_t> DomTree::dfs(const Graph& graph) {
    return dfsWithExcluded(graph, -1);
}

std::vector<size_t> DomTree::dfsWithExcluded(const Graph& graph, int excludedId) {
    assert(excludedId < static_cast<int>(graph.size()));
    std::vector<size_t> res;
    BasicBlock* startBlock = graph.blocks_[0];
    std::vector<bool> vis(graph.size(), false);
    if (excludedId >= 0) vis[excludedId] = true;
    std::stack<BasicBlock*> st;
    st.push(startBlock);
    while (!st.empty()) {
        auto blockPtr = st.top();
        st.pop();
        if (vis[blockPtr->id_]) continue;
        vis[blockPtr->id_] = true;
        res.push_back(blockPtr->id_);
        if (blockPtr->firstSuccs_ && !vis[blockPtr->firstSuccs_->id_]) st.push(blockPtr->firstSuccs_);
        if (blockPtr->secondSuccs_&& !vis[blockPtr->secondSuccs_->id_]) st.push(blockPtr->secondSuccs_);
    }
    return res;
}

std::vector<size_t> DomTree::calcRemainingBlocks(const std::vector<size_t>& dfsResult, size_t numBlocks) {
    std::vector<size_t> res;
    res.reserve(numBlocks - dfsResult.size());
    for (size_t i = 0; i < numBlocks; ++i) {
        if (std::find(dfsResult.begin(), dfsResult.end(), i) == dfsResult.end()) res.push_back(i);
    }
    return res;
}

std::vector<std::vector<size_t>> DomTree::createReverseGraph(const std::vector<std::vector<size_t>>& graph) {
    std::vector<std::vector<size_t>> reverseGraph(graph.size());
    for (size_t i = 0; i < graph.size(); ++i) {
        for (auto n : graph[i]) {
            reverseGraph[n].push_back(i);
        }
    }
    return reverseGraph;
}

std::vector<std::vector<size_t>> DomTree::calcDominated(const Graph& graph) {
    std::vector<std::vector<size_t>> dominated(graph.size());
    dominated[0] = dfs(graph);
    dominated[0].erase(std::find(dominated[0].begin(), dominated[0].end(), 0));
    if (dominated[0].size() != graph.size() - 1) {
        std::cout << "Unlinked graph!" << std::endl;
        return dominated;
    }
    for (size_t i = 1; i < graph.size(); ++i) {
        dominated[i] = calcRemainingBlocks(dfsWithExcluded(graph, i), graph.size());
        dominated[i].erase(std::find(dominated[i].begin(), dominated[i].end(), i));
    }
    return dominated;
}

std::vector<std::vector<size_t>> DomTree::calcDominators(const std::vector<std::vector<size_t>>& dominated) {
    return createReverseGraph(dominated);
}

}  // namespace Compiler