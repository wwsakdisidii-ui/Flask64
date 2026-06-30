#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>

struct N64Header {
    uint8_t  validationByte;
    uint8_t  compressionFlags;
    uint8_t  padding;
    uint8_t  clockRate;
    uint32_t programCounter;
    uint32_t releaseAddress;
    uint32_t crc1;
    uint32_t crc2;
    char     gameTitle[20];
    uint32_t mediaFormat;
    char     cartridgeId[2];
    uint8_t  countryCode;
    uint8_t  version;
};

class N64Rom {
public:
    std::vector<uint8_t> romData;
    N64Header header;

    bool LoadROM(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "Failed to locate ROM target file: " << filePath << std::endl;
            return false;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        romData.resize(size);

        if (!file.read(reinterpret_cast<char*>(romData.data()), size)) return false;

        // Auto-Detect Format and Convert to Big Endian Native (.z64)
        uint32_t magic = (romData[0] << 24) | (romData[1] << 16) | (romData[2] << 8) | romData[3];
        
        if (magic == 0x37804012) { // .v64 format (Byte Swapped)
            for (size_t i = 0; i < romData.size(); i += 2) {
                std::swap(romData[i], romData[i + 1]);
            }
        } else if (magic == 0x40123780) { // .n64 format (Little Endian)
            for (size_t i = 0; i < romData.size(); i += 4) {
                std::swap(romData[i], romData[i + 3]);
                std::swap(romData[i + 1], romData[i + 2]);
            }
        } // 0x80371240 is standard Big Endian (.z64), no swapping needed.

        // Map data structures directly into the header struct
        std::memcpy(&header, romData.data(), sizeof(N64Header));
        std::cout << "Successfully Loaded ROM: " << header.gameTitle << std::endl;
        return true;
    }
};
