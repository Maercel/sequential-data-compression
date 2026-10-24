//
// Created by Marcel on 24/10/2026.
//

#include "BitReader.h"

BitReader::BitReader(const std::vector<uint8_t>& bytes) : bytes(bytes) {}

uint16_t BitReader::read(uint8_t bitCount) {
    uint16_t value = 0;
    for (int i = 0; i < bitCount; i++) {
        uint8_t byte = bytes[pos / 8];
        uint8_t bit = (byte >> (7 - pos % 8)) & 1;
        value = (value << 1) | bit; // shifts 011 -> 011|bit 0
        pos++;
    }
    return value;
}