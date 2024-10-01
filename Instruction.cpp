#include "Instruction.hpp"

Instruction::Instruction(size_t id, InstType type, OpCode opCode, const std::vector<size_t> inputs, const std::vector<size_t> users) : id_(id), type_(type), opCode_(opCode), inputs_(inputs), users_(users) {}  

void Instruction::addInput(size_t id) {
    inputs_.push_back(id);
}

void Instruction::addInput(const std::vector<size_t>& ids) {
    for (const auto id : ids) {
        inputs_.push_back(id);
    }
}

void Instruction::addUser(size_t id) {
    users_.push_back(id);
}

void Instruction::addUser(const std::vector<size_t>& ids) {
    for (const auto id : ids) {
        users_.push_back(id);
    }
}