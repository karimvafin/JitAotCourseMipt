#ifndef RPO_HPP
#define RPO_HPP

#include "Graph.hpp"
namespace Compiler {

void RPOImpl(const BasicBlock* block, size_t& blocksCount, std::vector<size_t>& rpoVector, std::vector<bool>& vis);

std::vector<size_t> RPO(const Graph& graph);

}  // namespace Compiler

#endif  // RPO_HPP