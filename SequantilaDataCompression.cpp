//
// Created by Marcel on 24/10/2026.
//

#include "SequantilaDataCompression.h"

#include <algorithm>
#include <stdexcept>

#include "BitReader.h"
#include "BitWriter.h"

std::vector<uint8_t> SequentialDataCompression::compress(const std::vector<uint8_t>& data) {
    if (data.empty()) throw std::runtime_error("Empty data vector");

    const std::vector<int16_t> subtractedVals = computeDifferences(data);

    BitWriter writer;
    writer.write(data[0], 8);

    size_t i = 1;
    while (i < subtractedVals.size()) {
        const int16_t diff = subtractedVals[i];

        if (diff == 0) {
            uint8_t count = 0;
            while (i < subtractedVals.size() && subtractedVals[i] == 0 && count < 8) {
                count++;
                i++;
            }
            writer.write(0b01, 2);
            writer.write(count - 1, 3); // starting at 000, count = 1 -> 0
            continue;
        }

        if (uint8_t absDiff = std::abs(diff); absDiff > 30) {
            writer.write(0b10, 2);
            writer.write(diff < 0 ? 1 : 0, 1);
            writer.write(absDiff, 8);
        }

        else {
            uint8_t bits;
            if (absDiff <= 2) bits = 2;
            else if (absDiff <= 6) bits = 3;
            else if (absDiff <= 14) bits = 4;
            else bits = 5;

            const uint8_t code = diff < 0
                ? diff + (1 << bits) - 2
                : diff + 1;

            writer.write(0b00, 2);
            writer.write(bits - 2, 2); // -2 so we get 2-2 = 00 -> 0, 3 - 2 = 01 -> 1 ...
            writer.write(code, bits);
        }
        ++i;
    }

    writer.write(0b11, 2);
    return writer.data();
}

std::vector<uint8_t> SequentialDataCompression::decompress(const std::vector<uint8_t> &data) {
    BitReader reader(data);
    std::vector<uint8_t> values;

    auto current = static_cast<int16_t>(reader.read(8));
    values.push_back(current);

    while (true) {
         uint8_t rule = reader.read(2);

        if (rule == 0b00) {
            const uint8_t bits = reader.read(2) + 2; // group 00→2, 01→3, 10→4, 11→5
            const uint8_t code = reader.read(bits);

            const uint8_t half = 1 << (bits - 1); // the most significant bit is set to 1 for negative values and 0 for positive values
            const int16_t diff = code < half
                ? static_cast<int16_t>(code - ((1 << bits) - 2))
                : static_cast<int16_t>(code - 1);

            current = static_cast<int16_t>(current + diff);
            values.push_back(current);
        }
        else if (rule == 0b01) {
            const uint8_t repeats = reader.read(3) + 1;
            for (int i = 0; i < repeats; i++)
                values.push_back(current);
        }
        else if (rule == 0b10) {
            const uint8_t sign = reader.read(1);
            const uint8_t value = reader.read(8);
            current = static_cast<int16_t>(current + (sign ? -value : value));
            values.push_back(current);
        }
        else {
            break;
        }
    }

    return values;
}

std::vector<int16_t> SequentialDataCompression::computeDifferences(const std::vector<uint8_t> &data) {
    std::vector<int16_t> result(data.size());
    result[0] = data[0];

    for (size_t i = 1; i < data.size(); ++i)
        result[i] = static_cast<int16_t>(data[i] - data[i -1]);

    return result;
}
