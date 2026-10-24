#include <algorithm>
#include <iostream>
#include <array>
#include <cstdint>
#include <vector>
#include <random>
#include <bitset>
#include <iomanip>
#include <chrono>

#include "SequantilaDataCompression.h"

inline std::mt19937& rng() {
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}

std::vector<uint8_t> generateRandomValues(const int n, const uint8_t m) {
    std::vector<uint8_t> values;
    values.reserve(n);

    std::uniform_int_distribution<int> any(0, 255);

    uint8_t init = any(rng());
    values.push_back(init);

    for (int i = 1; i < n; ++i) {
        std::uniform_int_distribution<int> next(std::max(0, init - m),     // never below 0
                                                 std::min(255, init + m));

        init = next(rng());
        values.push_back(init);
    }

    return values;
}

void printTable(const std::string& title, const double table[4][5],
                const std::vector<int>& nValues, int precision) {
    std::cout << "\n" << title << "\n";
    std::cout << "N/M\t255\t5\t10\t15\t30\n";
    for (int r = 0; r < 4; r++) {
        std::cout << nValues[r];
        for (int c = 0; c < 5; c++)
            std::cout << '\t' << std::fixed << std::setprecision(precision) << table[r][c];
        std::cout << '\n';
    }
}

int main() {

    const std::vector<int> nValues = {5, 50, 500, 5000};
    const std::vector<int> mValues = {255, 5, 10, 15, 30};

    double ratio[4][5];
    double compressTime[4][5];
    double decompressTime[4][5];

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 5; c++) {
            std::vector<uint8_t> values = generateRandomValues(nValues[r], mValues[c]);
            std::vector<uint8_t> compressed;
            std::vector<uint8_t> decompressed;

            constexpr int repeats = 1000;
            auto start = std::chrono::steady_clock::now();
            for (int i = 0; i < repeats; i++)
                compressed = SequentialDataCompression::compress(values);
            auto end = std::chrono::steady_clock::now();
            compressTime[r][c] = std::chrono::duration<double, std::micro>(end - start).count() / repeats;

            start = std::chrono::steady_clock::now();
            for (int i = 0; i < repeats; i++)
                decompressed = SequentialDataCompression::decompress(compressed);
            end = std::chrono::steady_clock::now();
            decompressTime[r][c] = std::chrono::duration<double, std::micro>(end - start).count() / repeats;

            ratio[r][c] =  static_cast<double>(compressed.size()) / static_cast<double>(values.size());

            if (decompressed != values)
                std::cout << "ERROR: N=" << nValues[r] << " M=" << mValues[c] << " failed to decompress correctly!!\n";
        }
    }

    printTable("Compression ratio (compressed / original)", ratio, nValues, 2);
    printTable("Compression time [us]", compressTime, nValues, 3);
    printTable("Decompression time [us]", decompressTime, nValues, 3);
}