#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <iostream>

#include "BasicBlock.hpp"
#include "Indexer.hpp"

namespace Compiler {

class Function;

struct Graph {
private:
    BasicBlock* startBlock_ = nullptr;
    std::vector<BasicBlock*> blocks_;
    std::vector<Instruction*> insts_;
    std::vector<Value*> values_;
    std::unordered_map<size_t, BasicBlock*> id2Block_;
    std::unordered_map<size_t, Instruction*> id2Inst_;
    Indexer& blockIndexer_;  
    Indexer& instIndexer_;
    
public:
    Graph(Indexer& blockIndexer, Indexer& instIndexer): blockIndexer_(blockIndexer), instIndexer_(instIndexer) {}
    Graph(const Graph& other) = delete;
    Graph(Graph&& other) = delete;
    Graph& operator=(const Graph& other) = delete;
    Graph& operator=(Graph&& other) = delete;
    ~Graph();
    
    BasicBlock* createStartBlock();
    BasicBlock* createBasicBlock();
    void addBasicBlock(BasicBlock* block);
    void removeBasicBlock(BasicBlock* block);

    Instruction* createInst(InstType instType, OpCode opCode);
    Instruction* createInstConst(InstType instType, Value* value);
    Instruction* createInstStaticCall(Function* func);
    Instruction* copyInstruction(Instruction* inst);
    void addInstruction(Instruction* inst);
    void removeInstruction(Instruction* inst);

    void makeInstructionConst(Instruction* inst, Value* value);

    template <typename T>
    Value* createValue(T value);

    template <typename T>
    Value* createValue(T value, InstType instType);

    Value* createValueFromCopy(Value* value);

    Graph* copyCalleeGraph(const Graph* calleeGraph);

    size_t size() const {
        return blocks_.size();
    }

    size_t instructionCount() const {
        return insts_.size();
    }

    const std::unordered_map<size_t, Instruction*>& id2Inst() const {
        return id2Inst_;
    }

    const std::unordered_map<size_t, BasicBlock*>& id2Block() const {
        return id2Block_;
    }

    BasicBlock* startBlock() const {
        return startBlock_;
    }

    const std::vector<BasicBlock*>& blocks() const {
        return blocks_;
    }

    void print() const {
        std::cout << "----------------------------" << std::endl;
        for (auto b: blocks_) {
            std::cout << "Block " << b->id_ << std::endl;
            for (auto i: b->insts_) {
                std::cout << "Inst " << i->id_ << " " << opCode2Str(i->opCode_) << " " << "{ ";
                for (auto inp: i->inputs_) {
                    std::cout << inp->id_ << " ";
                }
                std::cout << "} -> { ";
                for (auto user: i->users_) {
                    std::cout << user->id_ << " ";
                }
                std::cout << "}" << std::endl;
            }
            std::cout << "\n";
        }
        std::cout << "----------------------------" << std::endl;
    }
};

template <typename T> 
Value* Graph::createValue(T value) {
    Value* val = new Value(value);
    values_.push_back(val);
    return val;
}

template <typename T> 
Value* Graph::createValue(T value, InstType instType) {
    Value* val;
    switch (instType) {
    case InstType::S32:
        val = new Value(static_cast<int32_t>(value));
        break;
    case InstType::S64:
        val = new Value(static_cast<int64_t>(value));
        break;
    case InstType::U32:
        val = new Value(static_cast<uint32_t>(value));
        break;
    case InstType::U64:
        val = new Value(static_cast<uint64_t>(value));
        break;
    case InstType::B:
        val = new Value(static_cast<bool>(value));
        break;
    default:
        break;
    }
    values_.push_back(val);
    return val;
}

}  // namespace Compiler

#endif  // GRAPH_HPP
