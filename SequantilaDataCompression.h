//
// Created by Marcel on 24/10/2026.
//

#ifndef SEQUENTIAL_DATA_COMPRESSION_SEQUANTILADATACOMPRESSION_H
#define SEQUENTIAL_DATA_COMPRESSION_SEQUANTILADATACOMPRESSION_H

#include <vector>
#include <cstdint>

class SequentialDataCompression {
    public:
    static std::vector<uint8_t> compress(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> decompress(const std::vector<uint8_t>& data);

private:
    static std::vector<int16_t> computeDifferences(const std::vector<uint8_t>& data);
};

#endif //SEQUENTIAL_DATA_COMPRESSION_SEQUANTILADATACOMPRESSION_H