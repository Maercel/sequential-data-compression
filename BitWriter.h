//
// Created by Marcel on 24/10/2026.
//

#ifndef SEQUENTIAL_DATA_COMPRESSION_BITWRITTER_H
#define SEQUENTIAL_DATA_COMPRESSION_BITWRITTER_H
#include <cstdint>
#include <vector>

class BitWriter {
public:
    void write(uint16_t value, uint8_t bitCount);
    const std::vector<uint8_t>& data() const { return bytes; }

private:
    std::vector<uint8_t> bytes;
    std::uint8_t bitPos{};
};

#endif //SEQUENTIAL_DATA_COMPRESSION_BITWRITTER_H