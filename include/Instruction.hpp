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
    PHI
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
    std::vector<size_t> inputs_;
    std::vector<size_t> users_;

    Instruction(size_t id, InstType type, OpCode opCode, const std::vector<size_t> inputs = std::vector<size_t>{}, const std::vector<size_t> users = std::vector<size_t>{});

    void addInput(size_t id);
    void addInput(const std::vector<size_t>& ids);

    void addUser(size_t id);
    void addUser(const std::vector<size_t>& ids);

};

}  // namespace Compiler

#endif  // INSTRUCTION_HPP