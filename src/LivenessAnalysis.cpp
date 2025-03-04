#include "LivenessAnalysis.hpp"

namespace Compiler {

Intervals buildIntervals(const std::vector<BasicBlock*> &basicBlocksOrder, const Graph& graph) {
    Intervals intervals;
    for (int blockInd = basicBlocksOrder.size() - 1; blockInd >= 0; --blockInd) {
        BasicBlock* blockPtr = basicBlocksOrder[blockInd];
        std::vector<size_t> live;
        if (blockPtr->firstSuccs_) {
            live.insert(live.end(), blockPtr->firstSuccs_->liveIn_.begin(), blockPtr->firstSuccs_->liveIn_.end());
            for (auto phiPtr: blockPtr->firstSuccs_->phiInsts_) {
                for (auto phiInputPtr: phiPtr->inputs_) {
                    if (blockPtr->hasInstruction(phiInputPtr->id_)) live.push_back(phiInputPtr->id_);
                }
            }
        }
        if (blockPtr->secondSuccs_) {
            live.insert(live.end(), blockPtr->secondSuccs_->liveIn_.begin(), blockPtr->secondSuccs_->liveIn_.end());
            for (auto phiPtr: blockPtr->secondSuccs_->phiInsts_) {
                for (auto phiInputPtr: phiPtr->inputs_) {
                    if (blockPtr->hasInstruction(phiInputPtr->id_)) live.push_back(phiInputPtr->id_);
                }
            }
        }

        for (auto opd: live) {
            intervals.addRange(opd, {blockPtr->insts_[0]->id_, blockPtr->insts_.back()->id_});
        }

        std::vector<Instruction*> &insts = blockPtr->insts_;
        for (int opInd = insts.size() - 1; opInd >= 0; --opInd) {
            Instruction* inst = insts[opInd];
            size_t outputId = inst->id_;
            intervals.setFrom(outputId, outputId);
            auto findOutputIdInLive = std::find(live.begin(), live.end(), outputId);
            if (findOutputIdInLive != live.end()) {
                live.erase(findOutputIdInLive);
            }

            std::vector<Instruction*> &inputInsts = inst->inputs_;
            for (auto inputPtr: inputInsts) {
                size_t inputId = inputPtr->id_;
                intervals.addRange(inputId, {blockPtr->insts_[0]->id_, inst->id_});
                live.push_back(inputId);
            }
        }

        for (auto phiPtr: blockPtr->phiInsts_) {
            auto findIdInLive = std::find(live.begin(), live.end(), phiPtr->id_);
            if (findIdInLive != live.end()) {
                live.erase(findIdInLive);
            }
        }

        blockPtr->liveIn_ = live;
    }
    return intervals;
}

} // namespace Compiler