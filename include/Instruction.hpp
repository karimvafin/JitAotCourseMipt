#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <vector>

#include "Value.hpp"

namespace Compiler {

enum class OpCode {
    ADD,
    SUB,
    MUL,
    JMP,
    JMPC,
    RET,
    CAST,
    CMP,
    MOV,
    PHI,
    SHR,
    OR,
    CNST,
    PRM,
    CALL
};

enum class InstType {
    S32,
    S64,
    U32,
    U64,
    B,
    VOID
};

unsigned int getInstTypeSize(InstType type);

class Function;

struct Instruction {
    size_t id_;
    InstType type_;
    OpCode opCode_;
    std::vector<Instruction*> inputs_;
    std::vector<Instruction*> users_;
    Value* value_ = nullptr;
    Function* callee_ = nullptr;

    Instruction(size_t id, InstType type, OpCode opCode);
    Instruction(size_t id, InstType type, OpCode opCode, Value* value);
    Instruction(size_t id, InstType type, Function* callee);

    void addInput(Instruction* inst);
    void addInput(const std::vector<Instruction*>& insts);
    void deleteInput(Instruction* inst);
    void swapInput(Instruction* oldInst, Instruction* newInst);
    void clearInput();

private:
    void addUser(Instruction* inst);
    void addUser(const std::vector<Instruction*>& insts);
    void deleteUser(Instruction* inst);

};

std::string opCode2Str(OpCode opCode);

}  // namespace Compiler

#endif  // INSTRUCTION_HPP
