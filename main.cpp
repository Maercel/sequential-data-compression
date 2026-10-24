#include <algorithm>
#include <iostream>
#include <array>
#include <cstdint>
#include <vector>
#include <random>

inline std::mt19937& rng() {
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}

std::vector<uint8_t> generateRandomValues(const uint8_t n, const uint8_t m) {
    std::vector<uint8_t> values;
    values.reserve(n);

    std::uniform_int_distribution<int> any(0, 255);

    uint8_t init = any(rng());
    values.push_back(init);

    for (int i = 1; i < n; ++i) {
        std::uniform_int_distribution<uint8_t> next(init - m, init + m);
        values.push_back(next(rng()));
    }

    return values;
}

int main() {

    std::vector<uint8_t> values = generateRandomValues(5, 5);
    std::ranges::for_each(values.cbegin(), values.cend(), [] (const auto v) {
        std::cout << (int)v << " ";
    });
}