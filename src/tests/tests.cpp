#include <iostream>

#include "run_all_stream_tests.hpp"

int main() {
    static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "Bad endian");

    bool ok(run_all_stream_tests());
    if (ok) {
        std::cout << "All tests passed" << std::endl;
    }
    return ok ? 0 : 1 ;
}


// vim: set shiftwidth=4 softtabstop=4 :
