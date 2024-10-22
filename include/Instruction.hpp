#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <vector>

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
    PRM  // parameter
};

enum class InstType {
    S32,
    S64,
    U32,
    U64,
    B,
    VOID
};


struct Instruction {
    size_t id_;
    InstType type_;
    OpCode opCode_;
    std::vector<Instruction*> inputs_;
    std::vector<Instruction*> users_;

    Instruction(size_t id, InstType type, OpCode opCode, const std::vector<Instruction*> inputs = std::vector<Instruction*>{}, const std::vector<Instruction*> users = std::vector<Instruction*>{});

    void addInput(Instruction* inst);
    void addInput(const std::vector<Instruction*>& insts);

private:
    void addUser(Instruction* inst);
    void addUser(const std::vector<Instruction*>& insts);

};

}  // namespace Compiler

#endif  // INSTRUCTION_HPP