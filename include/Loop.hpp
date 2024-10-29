#ifndef LOOP_HPP
#define LOOP_HPP

#include <utility>
#include <unordered_set>

namespace Compiler {
    
    struct Loop {
        size_t header_;
        std::vector<size_t> backEdges_;
        bool reducible_;
        std::vector<size_t> blocks_;
        std::vector<size_t> latches_;
        size_t id_;
        std::unordered_set<size_t> otherLoops_;
    };
}  // namespace Compiler

#endif  // LOOP_HPP