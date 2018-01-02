#include <cassert>
#include <iostream>

#include "run_all_merge_tests.hpp"

#include "../InputStream2.hpp"
#include "../OutputStream2.hpp"
#include "../merge.hpp"

bool test_merge(size_t isc, AbstractInputstream * isv[], AbstractOutputstream & os) {
    assert(isc < 9);

    char in_filename[20];
    char test_filename[20];
    char reference_filename[20];

    for (size_t j = 0; j < isc; ++j) {
        snprintf(in_filename, 19, "tests/sorted.%lu", j+1);
        isv[j]->open(in_filename);
    }
    snprintf(test_filename, 19, "tests/merge.%lu.out", isc);
    os.create(test_filename);

    merge(isc, isv, os);

    for (size_t j = 0; j < isc; ++j) {
        isv[j]->close();
    }
    os.close();

    snprintf(reference_filename, 19, "tests/merged.%lu", isc);
    InputStream2 reference;
    reference.open(reference_filename);

    InputStream2 test;
    test.open(test_filename);

    while (!reference.end_of_stream() && !test.end_of_stream()) {
        if (reference.read_next() != test.read_next()) {
            return false;
        }
    }
    if (!reference.end_of_stream() || !test.end_of_stream()) {
        return false;
    }

    return true;
}

bool run_all_merge_tests() {
    bool pass(true);
    AbstractInputstream * isv[3];
    OutputStream2 os;

    for (size_t i = 0; i < 3; ++i) {
        isv[i] = new InputStream2();
        if (!test_merge(i+1, isv, os)) {
            std::cout << "Merge " << i+1 << " streams failed" << std::endl;
            pass = false;
        }

    }

    return pass;
}
// vim: set shiftwidth=4 softtabstop=4 :
