#ifndef LOOPANALYZER_HPP
#define LOOPANALYZER_HPP

#include <list>
#include <unordered_map>

#include "Graph.hpp"
#include "Loop.hpp"

namespace Compiler {

struct LoopAnalyzer {
    using LoopIt = std::list<Loop>::iterator;

    Graph *graph_;
    std::list<Loop> loops_;
    std::unordered_map<size_t, LoopIt> idLoop_;
    std::unordered_map<size_t, LoopIt> headerLoop_;

    LoopAnalyzer(Graph* graph) : graph_(graph) {}

    void findBackEdges();
    void populateLoops();
    void createRootLoop();
};
    

}


#endif  // LOOPANALYZER_HPP