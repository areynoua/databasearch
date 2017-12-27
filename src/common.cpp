#include <cstdint>

#include "common.hpp"

int_least32_t charsToInt32 (const char* const chars) {
    return (static_cast<int_least32_t>(chars[0]))
        | (static_cast<int_least32_t>(chars[1]) << 8)
        | (static_cast<int_least32_t>(chars[2]) << 16)
        | (static_cast<int_least32_t>(chars[3]) << 24);
}

void int32ToChars (char dest[4], const int_least32_t& number) {
    dest[0] = static_cast<char>(number);
    dest[1] = static_cast<char>(number >> 8);
    dest[2] = static_cast<char>(number >> 16);
    dest[3] = static_cast<char>(number >> 24);
}

#include <iostream>

void print_all(AbstractInputstream& is) {
    while (!is.end_of_stream()) {
        std::cout << is.read_next() << std::endl;;
    }
}

