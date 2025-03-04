#include <iostream>
#include <assert.h>

#include "RPO.hpp"
#include "LivenessAnalysis.hpp"

using namespace Compiler;

void test1(bool verbose = false) {
    /** Graph construction */
    Graph* graph = new Graph();

    /** Basic Blocks construction */
    BasicBlock* bb0 = new BasicBlock(0, graph); // start
    BasicBlock* bb1 = new BasicBlock(1, graph); // true
    BasicBlock* bb2 = new BasicBlock(2, graph); // false
    BasicBlock* bb3 = new BasicBlock(3, graph); // ret

    /** Instructions construction */
    int i = 0;
    Instruction* inst00 = new Instruction(i++, InstType::S32, OpCode::PRM);  // int a = param1;
    Instruction* inst01 = new Instruction(i++, InstType::S32, OpCode::MOV);  // int c = 1;
    Instruction* inst02 = new Instruction(i++, InstType::B, OpCode::CMP);    // bool q = a > c;
    Instruction* inst03 = new Instruction(i++, InstType::VOID, OpCode::JMPC); 

    Instruction* inst10 = new Instruction(i++, InstType::S32, OpCode::MOV);  // int b1 = 2;
    Instruction* inst11 = new Instruction(i++, InstType::VOID, OpCode::JMP); 

    Instruction* inst20 = new Instruction(i++, InstType::S32, OpCode::MOV);  // int b2 = 3;
    Instruction* inst21 = new Instruction(i++, InstType::VOID, OpCode::JMP); 

    Instruction* inst30 = new Instruction(i++, InstType::S32, OpCode::PHI);  // int b = phi(b1, b2);
    Instruction* inst31 = new Instruction(i++, InstType::S32, OpCode::ADD);  // int r = a + b;
    Instruction* inst32 = new Instruction(i++, InstType::S32, OpCode::RET);  // return r;

    /** Add inputs */
    inst02->addInput({inst00, inst01});
    inst03->addInput(inst02);
    inst30->addInput({inst10, inst20});
    inst31->addInput({inst00, inst30});
    inst32->addInput(inst31);

    /** Add instructions to basic blocks */
    bb0->addInstructionToEnd({inst00, inst01, inst02, inst03});
    bb1->addInstructionToEnd({inst10, inst11});
    bb2->addInstructionToEnd({inst20, inst21});
    bb3->addInstructionToEnd({inst30, inst31, inst32});

    /** Add predecessors and successors to basic blocks */
    bb0->addSuccessor(bb1, true);
    bb0->addSuccessor(bb2, false);

    bb1->addSuccessor(bb3, true);

    bb2->addSuccessor(bb3, true);

    /** Add basic blocks to graph */
    graph->addBasicBlock({bb0, bb1, bb2, bb3});

    /** TESTS */
    assert((bb0->getInstructionIds() == std::vector<size_t>{0, 1, 2, 3}));
    assert((bb1->getInstructionIds() == std::vector<size_t>{4, 5}));
    assert((bb2->getInstructionIds() == std::vector<size_t>{6, 7}));
    assert((bb3->getInstructionIds() == std::vector<size_t>{8, 9, 10}));

    auto intervals = buildIntervals({bb0, bb1, bb2, bb3}, *graph);

    for (auto pair: intervals.intervals) {
        std::cout << "Id: " << pair.first << ". Ranges: ";
        for (auto range: pair.second) {
            std::cout << range.first << "-" << range.second << ", ";
        }
        std::cout << std::endl;
    }
    
    delete graph;
    delete bb0; delete bb1; delete bb2; delete bb3;
    delete inst00; delete inst01; delete inst02; delete inst03; delete inst10; delete inst11; 
    delete inst20; delete inst21; delete inst30; delete inst31; delete inst32; 
}

int main() {
    test1(true);
}