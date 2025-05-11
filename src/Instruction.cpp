#include "Instruction.hpp"
#include "Function.hpp"

namespace Compiler {

Instruction::Instruction(size_t id, InstType type, OpCode opCode) : id_(id), type_(type), opCode_(opCode) {}  
Instruction::Instruction(size_t id, InstType type, OpCode opCode, Value* value) : id_(id), type_(type), opCode_(opCode), value_(value) {}  
Instruction::Instruction(size_t id, InstType type, Function* callee) : id_(id), type_(type), opCode_(OpCode::CALL), callee_(callee) {}  

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

void Instruction::deleteInput(Instruction* inst) {
    inst->deleteUser(this);
    inputs_.erase(std::remove(inputs_.begin(), inputs_.end(), inst), inputs_.end());
}

void Instruction::swapInput(Instruction* oldInst, Instruction* newInst) {
    oldInst->deleteUser(this);
    auto it = std::find(inputs_.begin(), inputs_.end(), oldInst);
    assert(it != inputs_.end());
    *it = newInst;
    newInst->addUser(this);
}

void Instruction::clearInput() {
    for (auto &inst: inputs_) {
        inst->deleteUser(this);
    }
    inputs_.clear();
}

void Instruction::addUser(Instruction* inst) {
    users_.push_back(inst);
}

void Instruction::addUser(const std::vector<Instruction*>& insts) {
    for (const auto inst : insts) {
        users_.push_back(inst);
    }
}

void Instruction::deleteUser(Instruction* inst) {
    users_.erase(std::remove(users_.begin(), users_.end(), inst), users_.end());
}

std::string opCode2Str(OpCode opCode) {
    if (opCode == OpCode::ADD) return "ADD";
    if (opCode == OpCode::SUB) return "SUB";
    if (opCode == OpCode::JMP) return "JMP";
    if (opCode == OpCode::JMPC) return "JMPC";
    if (opCode == OpCode::RET) return "RET";
    if (opCode == OpCode::CAST) return "CAST";
    if (opCode == OpCode::CMP) return "CMP";
    if (opCode == OpCode::MOV) return "MOV";
    if (opCode == OpCode::PHI) return "PHI";
    if (opCode == OpCode::SHR) return "SHR";
    if (opCode == OpCode::OR) return "OR";
    if (opCode == OpCode::CNST) return "CNST";
    if (opCode == OpCode::PRM) return "PRM";
    if (opCode == OpCode::CALL) return "CALL";
    return "UNKNOWN";
}

unsigned int getInstTypeSize(InstType type) {
    switch (type) {
    case InstType::S32:
        return 32;
    case InstType::S64:
        return 64;
    case InstType::U32:
        return 32;
    case InstType::U64:
        return 64;
    default:
        assert(false);
        break;
    }
}

}  // namespace Compiler
