#pragma once
#include "Memory.hpp"
#include <iostream>

class VR4300CPU {
public:
    uint64_t registers[32] = {0};
    uint64_t pc = 0x80000400; // Standard initial N64 vector entry point
    N64Memory* memory;

    VR4300CPU(N64Memory* memSystem) : memory(memSystem) {}

    void Step() {
        // Fetch 32-bit operation block
        uint32_t instruction = memory->Read32(static_cast<uint32_t>(pc));
        
        uint8_t opCode = (instruction >> 26) & 0x3F;
        uint8_t rs = (instruction >> 21) & 0x1F;
        uint8_t rt = (instruction >> 16) & 0x1F;
        uint8_t rd = (instruction >> 11) & 0x1F;
        uint16_t immediate = instruction & 0xFFFF;

        switch (opCode) {
            case 0x00: // SPECIAL execution matrix shifts
                ExecuteSpecial(instruction, rs, rt, rd);
                break;
            case 0x0F: // LUI (Load Upper Immediate) - Common in game boot sequences
                registers[rt] = static_cast<int64_t>(static_cast<int16_t>(immediate)) << 16;
                pc += 4;
                break;
            case 0x0D: // ORI (Bitwise OR Immediate)
                registers[rt] = registers[rs] | immediate;
                pc += 4;
                break;
            default:
                // Fallback increment for unassigned/WIP opcodes to prevent infinite cycles
                pc += 4;
                break;
        }
    }

private:
    void ExecuteSpecial(uint32_t instr, uint8_t rs, uint8_t rt, uint8_t rd) {
        uint8_t function = instr & 0x3F;
        switch (function) {
            case 0x25: // OR operation
                registers[rd] = registers[rs] | registers[rt];
                break;
        }
        pc += 4;
    }
};
