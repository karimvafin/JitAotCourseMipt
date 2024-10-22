#include "Instruction.hpp"

namespace Compiler {

Instruction::Instruction(size_t id, InstType type, OpCode opCode, const std::vector<Instruction*> inputs, const std::vector<Instruction*> users) : id_(id), type_(type), opCode_(opCode), inputs_(inputs), users_(users) {}  

void Instruction::addInput(Instruction* inst) {
    inputs_.push_back(inst);
    inst->addUser(this);
}

void Instruction::addInput(const std::vector<Instruction*>& insts) {
    for (const auto inst : insts) {
        inputs_.push_back(inst);
        inst->addUser(this);
    }
}

void Instruction::addUser(Instruction* inst) {
    users_.push_back(inst);
}

void Instruction::addUser(const std::vector<Instruction*>& insts) {
    for (const auto inst : insts) {
        users_.push_back(inst);
    }
}

}  // namespace Compiler