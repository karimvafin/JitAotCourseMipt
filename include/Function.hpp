#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "Graph.hpp"
#include "Value.hpp"

namespace Compiler {

struct Function {
    Graph* graph_;
    std::string name_;
    InstType retType_;

    Function(Graph* graph, const std::string& name, InstType retType): graph_(graph), name_(name), retType_(retType) {}

    size_t instructionCount() const {
        return graph_->instructionCount();
    }

    Graph* graph() {
        return graph_;
    }

    ~Function() {
        delete graph_;
    }
};

Function* createFunction(Graph* graph, const std::string& name, InstType retType);

} // namespace Compiler

#endif // FUNCTION_HPP