//
// Created by Marcel on 24/10/2026.
//

#ifndef SEQUENTIAL_DATA_COMPRESSION_BITREADER_H
#define SEQUENTIAL_DATA_COMPRESSION_BITREADER_H

#include <vector>
#include <cstdint>

class BitReader {
public:
    explicit BitReader(const std::vector<uint8_t>& bytes);

    uint16_t read(uint8_t bitCount);

private:
    const std::vector<uint8_t>& bytes;
    size_t pos = 0;
};

#endif //SEQUENTIAL_DATA_COMPRESSION_BITREADER_H