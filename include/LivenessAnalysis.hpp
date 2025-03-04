#ifndef LIVENESSANALYSIS_HPP
#define LIVENESSANALYSIS_HPP

#include <unordered_map>
#include <iostream>
#include <unordered_set>

#include "BasicBlock.hpp"
#include "Graph.hpp"

namespace Compiler {

struct Intervals {
    std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> intervals;

    void addRange(size_t id, const std::pair<size_t, size_t> &range) {
        auto it = intervals.find(id);
        if (it != intervals.end()) {
            intervals[id].push_back(range);
        } else {
            std::vector<std::pair<size_t, size_t>> ranges = {range};
            intervals[id] = ranges;
        }
    }

    void setFrom(size_t id, size_t from) {
        auto it = intervals.find(id);
        if (it == intervals.end()) {
            std::cout << "Error! No such opd. Id: " << id << std::endl; 
        } else {
            intervals[id][0].first = from;
        }
    }
};

Intervals buildIntervals(const std::vector<BasicBlock*> &basicBlocksOrder, const Graph& graph);

} // namespace Compiler

#endif // LIVENESSANALYSIS_HPP