#include "Memory.hpp"
#include "Cpu.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: N64Emulator <Path to Rom Hack File (.z64)>" << std::endl;
        return -1;
    }

    N64Memory systemMemory;
    if (!systemMemory.activeRom.LoadROM(argv[1])) {
        return -1;
    }

    VR4300CPU systemCpu(&systemMemory);
    systemCpu.pc = systemMemory.activeRom.header.programCounter; // Bypass internal PIF checks

    std::cout << "Emulator initialized successfully. Booting execution context..." << std::endl;

    // Core Processing Execution Cycle
    bool running = true;
    while (running) {
        systemCpu.Step();
        // Hook UI/Render updates or breakpoints here
    }

    return 0;
}
