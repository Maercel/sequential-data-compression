//
// Created by Marcel on 24/10/2026.
//

#include "BitWriter.h"

void BitWriter::write(uint16_t value, uint8_t bitCount) {
    for (int i = bitCount - 1; i >= 0; i--) {
        uint8_t bit = (value >> i) & 1;
        if (bitPos == 0) {
            bytes.push_back(0);
        }
        bytes.back() |= bit << (7 - bitPos);
        bitPos = (bitPos + 1) % 8;
    }
}