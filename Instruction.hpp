#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

enum class OpCode {
    ADD,
    SUB,
    MUL,
    JMP,
    JMPC,
    RET,
    CAST,
    CMP,
    MOV
};

enum class InstType {
    S32,
    S64,
    U32,
    U64,
    B
};


struct Instruction {
    Instruction* prev_;
    Instruction* next_;
    int id_;
    bool isPhi_;
    OpCode opCode_;

};

#endif  // INSTRUCTION_HPP