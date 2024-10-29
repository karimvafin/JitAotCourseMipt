#include <stack>
#include <unordered_map>
#include <assert.h>

#include "LoopAnalyzer.hpp"
#include "DomTree.hpp"
#include "RPO.hpp"

#include <iostream>

namespace Compiler {

    enum Marker {
        GREY = 0,
        BLACK = 1
    };

    void LoopAnalyzer::findBackEdges() {
        // DFS
        std::vector<Marker> markers(graph_->size());

        BasicBlock* startBlock = graph_->blocks_[0];
        std::vector<bool> vis(graph_->size(), false);
        std::stack<BasicBlock*> st;
        st.push(startBlock);
        while (!st.empty()) {
            auto blockPtr = st.top();
            st.pop();
            if (vis[blockPtr->id_]) continue;
            markers[blockPtr->id_] = Marker::GREY;
            vis[blockPtr->id_] = true;

            // Check predecessors
            for (auto pred : blockPtr->preds_) {
                if ((pred->firstSuccs_    && vis[pred->firstSuccs_->id_]) && (pred->secondSuccs_ && vis[pred->secondSuccs_->id_]) ||
                    (!pred->firstSuccs_)  && (pred->secondSuccs_ && vis[pred->secondSuccs_->id_]) ||
                    (!pred->secondSuccs_) && (pred->firstSuccs_  && vis[pred->firstSuccs_->id_] )) {
                    // markers[pred->id_] = BLACK;
                }

            }
            if (blockPtr->firstSuccs_) {
                if (!vis[blockPtr->firstSuccs_->id_])
                    st.push(blockPtr->firstSuccs_);
                else {
                    if (markers[blockPtr->firstSuccs_->id_] == Marker::GREY) {
                        auto loop = headerLoop_.find(blockPtr->firstSuccs_->id_);
                        if (loop == headerLoop_.end()) { 
                            loops_.push_back(Loop{blockPtr->firstSuccs_->id_, {blockPtr->id_}});
                            loops_.back().id_ = loops_.size();
                            headerLoop_[blockPtr->firstSuccs_->id_] = std::prev(loops_.end());
                            idLoop_[loops_.back().id_] = std::prev(loops_.end());
                        } else {
                            loop->second->backEdges_.push_back(blockPtr->id_);
                        }
                    }
                }
            }
            if (blockPtr->secondSuccs_) {
                if (!vis[blockPtr->secondSuccs_->id_])
                    st.push(blockPtr->secondSuccs_);
                else {
                    if (markers[blockPtr->secondSuccs_->id_] == Marker::GREY) {
                        auto loop = headerLoop_.find(blockPtr->secondSuccs_->id_);
                        if (loop == headerLoop_.end()) { 
                            loops_.push_back(Loop{blockPtr->secondSuccs_->id_, {blockPtr->id_}});
                            loops_.back().id_ = loops_.size();
                            headerLoop_[blockPtr->secondSuccs_->id_] = std::prev(loops_.end());
                            idLoop_[loops_.back().id_] = std::prev(loops_.end());
                        } else {
                            loop->second->backEdges_.push_back(blockPtr->id_);
                        }
                    }
                }
            }
        }
    }

    void LoopAnalyzer::populateLoops() {
        size_t loopId = 0;
        auto rpoVector = RPO(*graph_);
        for (auto it = rpoVector.rbegin(), itEnd = rpoVector.rend(); it != itEnd; ++it) {
            auto headerIt = headerLoop_.find(*it);
            if (headerIt == headerLoop_.end()) continue;
            std::vector<bool> vis(graph_->size(), false);
            vis[*it] = true;
            headerIt->second->blocks_.push_back(*it);
            std::stack<BasicBlock*> st;
            for (auto source : headerIt->second->backEdges_)
                st.push(graph_->blocks_[source]);
            while (!st.empty()) {
                auto blockPtr = st.top();
                st.pop();
                if (vis[blockPtr->id_]) continue;
                vis[blockPtr->id_] = true;
                headerIt->second->blocks_.push_back(blockPtr->id_);
                for (auto loopId : blockPtr->loopIds_) 
                    headerIt->second->otherLoops_.insert(loopId);
                blockPtr->loopIds_.push_back(headerIt->second->id_);
                for (auto pred : blockPtr->preds_) {
                    if (!vis[pred->id_]) {
                        st.push(pred);
                    }
                }
            }
        }
    }

}