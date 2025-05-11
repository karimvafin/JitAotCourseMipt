#include "Function.hpp"

namespace Compiler {

Function* createFunction(Graph* graph, const std::string& name, InstType retType) {
    Function* f = new Function(graph, name, retType);
    return f;
}

} // namespace Compiler