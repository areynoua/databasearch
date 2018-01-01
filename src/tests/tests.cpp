#include "run_all_stream_tests.hpp"

int main() {
    static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "Bad endian");

    run_all_stream_tests();

    return 0;
}


// vim: set shiftwidth=4 softtabstop=4 :
