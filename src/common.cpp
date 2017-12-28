#include <cstdint>

#include "common.hpp"


// TODO: improve with memcopy or reinterpret_cast
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
// vim: set shiftwidth=4 softtabstop=4 :
