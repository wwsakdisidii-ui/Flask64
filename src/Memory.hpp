#pragma once
#include <vector>
#include <cstdint>
#include "Rom.hpp"

class N64Memory {
private:
    std::vector<uint8_t> rdram; // 8MB Expansion Pak enabled by default for ROM hacks
public:
    N64Rom activeRom;

    N64Memory() {
        rdram.resize(8 * 1024 * 1024, 0); // 8 Megabytes
    }

    uint32_t Read32(uint32_t address) {
        // Handle virtual address translations to physical mask locations
        uint32_t physicalAddress = address & 0x1FFFFFFF;

        // 1. Route to RDRAM space
        if (physicalAddress >= 0x00000000 && physicalAddress < 0x00800000) {
            return *reinterpret_cast<uint32_t*>(&rdram[physicalAddress]);
        }
        // 2. Route to Cartridge ROM Space
        else if (physicalAddress >= 0x10000000 && physicalAddress < 0x1FC00000) {
            uint32_t romOffset = physicalAddress - 0x10000000;
            if (romOffset < activeRom.romData.size()) {
                // Ensure Big Endian structure is read cleanly
                uint8_t* p = &activeRom.romData[romOffset];
                return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
            }
        }
        return 0; // Fallback for unmapped or dead I/O registers
    }

    void Write32(uint32_t address, uint32_t value) {
        uint32_t physicalAddress = address & 0x1FFFFFFF;

        if (physicalAddress >= 0x00000000 && physicalAddress < 0x00800000) {
            *reinterpret_cast<uint32_t*>(&rdram[physicalAddress]) = value;
        }
    }
};
