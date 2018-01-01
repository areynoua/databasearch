#include <iomanip>
#include <iostream>

#include "test_streams.hpp"

#include "../InputStream1.hpp"
#include "../OutputStream1.hpp"
#include "../InputStream2.hpp"
#include "../OutputStream2.hpp"
#include "../InputStream3.hpp"
#include "../OutputStream3.hpp"
#include "../InputStream4.hpp"
#include "../OutputStream4.hpp"

#include "testdata.hpp"

/**
 * Test `os` and `is`, `desc` is a short description included in report messages
 * 1. Use `os` to write a file identical to the 'testdata.bin' file (from content hard-coded in testdata.hpp) (does NOT check)
 * 2. Use `is` to read the 'testdata.bin' file (and check with the hard-coded content)
 * 3. Use `is` to read the data written by `os` (and check with the hard-coded content)
 * 4. Use `os` and `is` to create a copy of "random.65536" (does NOT check)
 *
 * The written files are named `desc` + ".out" and "random.cpy"
 */
void testStreams(AbstractOutputstream & os, AbstractInputstream & is, const char* const desc) {
    // out
    const char OUT_EXT[]{".out"};
    char * out_filename = new char[strlen(desc) + strlen(OUT_EXT) + 1];
    strcpy(out_filename, desc);
    strcat(out_filename, OUT_EXT);

    // in
    const size_t IN_FILES(2);
    const char * const IN_FILENAMES[IN_FILES]{TESTDATA_FILENAME, out_filename};

    // 1
    std::cout << "BEGIN: Output " << desc << " to   " << out_filename << "." << std::endl;

    os.create(out_filename);
    for (size_t i = 0; i < TESTDATA_SIZE; ++i) {
        os.write(TESTDATA[i]);
    }
    os.close();

    std::cout << "END  : Output " << desc << " to   " << out_filename << ".\n" << std::endl;

    // 2 and 3
    for (size_t f = 0; f < IN_FILES; ++f) {
        const char * const in_filename = IN_FILENAMES[f];
        std::cout << "BEGIN: Input  " << desc << " from " << in_filename << "." << std::endl;

        is.open(in_filename);
        size_t i = 0;
        while (!is.end_of_stream() && i < TESTDATA_SIZE) {
            int_least32_t read = is.read_next();
            if (TESTDATA[i] != read) {
                std::cerr << "       " << i << ": "
                    << std::setw(10) << TESTDATA[i] << " expected but " << std::setw(10) << read << " found."
                    << std::endl;
            }
            ++i;
        }
        if (i != TESTDATA_SIZE) {
            std::cerr << "       Did not stop at the end (at " << i << " instead of " << TESTDATA_SIZE << ")" << std::endl;
        }
        while (!is.end_of_stream()) {
            int_least32_t read = is.read_next();
            std::cerr << "       " << i << ": " << std::setw(10) << read << " found." << std::endl;
            ++i;
        }

        std::cout << "END  : Test input  " << desc << " from " << in_filename << ".\n" << std::endl;

        is.close();
    }

    // 4. open on another file
    is.open("random.65536");
    os.create("random.cpy");
    while (! is.end_of_stream()) {
        os.write(is.read_next());
    }
    os.close();
    is.close();
}

template <typename OS, typename IS>
void metaTestStreams(const char* const desc) {
    OS os;
    IS is;
    testStreams(os, is, desc);
}
