#ifndef CONSTANT_FOLDING_HPP
#define CONSTANT_FOLDING_HPP

#include "Instruction.hpp"
#include "Graph.hpp"

namespace Compiler {

void constantFoldingInstruction(Instruction* inst); 

void constantFolding(Graph* graph);

} // namespace Compiler

#endif // CONSTANT_FOLDING_HPP
