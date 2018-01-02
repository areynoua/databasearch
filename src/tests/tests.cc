#include <iostream>

#include "run_all_stream_tests.hpp"
#include "run_all_merge_tests.hpp"

const int A_NONE = 0x0;
const int A_STREAMS = 0x01;
const int A_MERGE = 0x02;
const int A_ERROR = 0x80;
const int A_ALL = 0xff;

int parse(int argc, char * argv[]) {
    if (argc <= 1) {
        return A_ALL;
    }

    int action = A_NONE;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "streams") == 0) {
            action |= A_STREAMS;
        }
        else if (strcmp(argv[1], "merge") == 0) {
            action |= A_MERGE;
        }
        else {
            action |= A_ERROR;
        }
    }

    return action;
}

int main(int argc, char * argv[]) {
    static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "Bad endian");

    int action = parse(argc, argv);
    bool ok(true);

    if ((action & A_NONE) || (action & A_ERROR)) {
        std::cout << "Usage:\n    " << argv[0] << " [streams] [merge]" << std::endl;
        ok = false;
    }
    if (action & A_STREAMS) {
        if (run_all_stream_tests()) {
            std::cout << "All streams tests passed" << std::endl;
        }
        else {
            ok = false;
        }
    }
    if (action & A_MERGE) {
        if (run_all_merge_tests()) {
            std::cout << "All merge tests passed" << std::endl;
        }
        else {
            ok = false;
        }
    }

    return ok ? 0 : 1 ;
}


// vim: set shiftwidth=4 softtabstop=4 :
