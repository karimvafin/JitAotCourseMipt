#ifndef INDEXER_HPP
#define INDEXER_HPP

namespace Compiler {

struct Indexer {
    size_t currIndex = 0;

    size_t getIndex() {
        size_t res = currIndex;
        currIndex++;
        return res;
    }
    
    Indexer(size_t startIdx = 0): currIndex(startIdx) {}
};

} // namespace Compiler

#endif // INDEXER_HPP
