#ifndef STATIC_INLINE_HPP
#define STATIC_INLINE_HPP

#include <unordered_set>

#include "Graph.hpp"

namespace Compiler {

void staticInline(Graph* graph);

void staticInlineImpl(Graph* graph, std::unordered_set<Graph*>& processed);

void inlineCallee(Graph* callerGraph, BasicBlock* block, Instruction* callInst, Function* callee);

} // namespace Compiler

#endif // STATIC_INLINE_HPP
