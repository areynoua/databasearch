#include <iostream>

#include "common.hpp"

void print_all(AbstractInputstream& is) {
    while (!is.end_of_stream()) {
        std::cout << is.read_next() << std::endl;;
    }
}
// vim: set shiftwidth=4 softtabstop=4 :
