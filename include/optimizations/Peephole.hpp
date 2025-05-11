#ifndef PEEPHOLE_HPP
#define PEEPHOLE_HPP

#include "Instruction.hpp"
#include "Graph.hpp"

namespace Compiler {

void peepholeInstruction(Instruction* inst, Graph* graph);

void peepholeOptimization(Graph* graph);

} // namespace Compiler

#endif // PEEPHOLE_HPP
