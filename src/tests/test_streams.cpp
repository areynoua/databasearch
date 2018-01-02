#include <iomanip>
#include <iostream>

#include "test_streams.hpp"

#include "../InputStream1.hpp"
#include "../OutputStream1.hpp"
#include "../InputStream2.hpp"
#include "../OutputStream2.hpp"

#include "testdata.hpp"

/**
 * Test `os` and `is`, `desc` is a short description included in report messages
 * 1. Use `os` to write a file identical to the 'testdata.bin' file (from content hard-coded in testdata.hpp)
 * 2. Use `is` to read the 'testdata.bin' file (and check with the hard-coded content)
 * 3. Use `is` to read the data written by `os` (and check with the hard-coded content)
 * 4. Use `os` and `is` to create a copy of "tests/random.65536" (does NOT check)
 *
 * The written files are named "tests/" + `desc` + ".data.out" and "tests/" + `desc` + "random.out"
 *
 * If check is true: checks are performed by std::system for the 4 steps and the output is more convenient
 * If check is false the first and last steps are not checked and the output is more verbose, adapted to check manually
 *
 * Returns wether all the check pass
 */
bool testStreams(AbstractOutputstream & os, AbstractInputstream & is, const char* const desc, bool check) {
    bool passed(true);
    char shortDesc[23];
    strncpy(shortDesc, desc, 23);
    shortDesc[23] = '\x0';

    if (check) {
        if (!std::system(nullptr)) {
            std::cout << "! Unable to perform all the tests" << std::endl;
            check = false;
        }
    }

    // 1
    char * out_filename = new char[41];
    snprintf(out_filename, 40, "tests/%s.data.out", shortDesc);

    if (!check)
        std::cout << "BEGIN: Output " << desc << " to   " << out_filename << "." << std::endl;

    os.create(out_filename);
    for (size_t i = 0; i < TESTDATA_SIZE; ++i) {
        os.write(TESTDATA[i]);
    }
    os.close();

    if (!check) std::cout << "END  : Output " << desc << " to   " << out_filename << ".\n" << std::endl;
    else {
        char * command = new char[91];
        snprintf(command, 90, "diff -q %s %s", TESTDATA_FILENAME, out_filename);
        int result(std::system(command));
        if (result != 0) {
            std::cout << "It seems that the test " << desc << " failed to write " << out_filename << " identical to " << TESTDATA_FILENAME << std::endl;
            passed = false;
        }
    }

    // 2 and 3
    const size_t IN_FILES(2);
    const char * const IN_FILENAMES[IN_FILES]{TESTDATA_FILENAME, out_filename};

    for (size_t f = 0; f < IN_FILES; ++f) {
        bool ok(true);
        const char * const in_filename = IN_FILENAMES[f];
        if (!check)
            std::cout << "BEGIN: Input  " << desc << " from " << in_filename << "." << std::endl;

        is.open(in_filename);
        size_t i = 0;
        while (!is.end_of_stream() && i < TESTDATA_SIZE) {
            int32_t read = is.read_next();
            if (TESTDATA[i] != read) {
                ok = false;
                if (!check) {
                    std::cout << "       " << i << ": "
                        << std::setw(10) << TESTDATA[i] << " expected but " << std::setw(10) << read << " found."
                        << std::endl;
                }
            }
            ++i;
        }
        if (i != TESTDATA_SIZE) {
            ok = false;
            if (!check) {
                std::cout << "       Did not stop at the end (at " << i << " instead of " << TESTDATA_SIZE << ")" << std::endl;
            }
        }
        while (!is.end_of_stream()) {
            ok = false;
            int32_t read = is.read_next();
            if (!check) {
                std::cout << "       " << i << ": " << std::setw(10) << read << " found." << std::endl;
            }
            ++i;
        }

        if (!check) std::cout << "END  : Input  " << desc << " from " << in_filename << ".\n" << std::endl;
        else {
            if (!ok) {
                std::cout << "It seems that the test " << desc << " failed to read " << in_filename << " identical to " << TESTDATA_FILENAME << std::endl;
                passed = false;
            }
        }

        is.close();
    }

    // 4
    snprintf(out_filename, 40, "tests/%s.random.out", shortDesc);

    if (!check)
        std::cout << "BEGIN: Input/output " << desc << " from tests/random.65536 to " << out_filename << "." << std::endl;
    is.open("tests/random.65536");
    os.create(out_filename);
    while (! is.end_of_stream()) {
        os.write(is.read_next());
    }
    os.close();
    is.close();
    if (!check) std::cout << "END  : Input/output " << desc << " from tests/random.65536 to " << out_filename << "." << std::endl;
    else {
        char * command = new char[91];
        snprintf(command, 90, "diff -q %s %s", "tests/random.65536", out_filename);
        int result(std::system(command));
        if (result != 0) {
            std::cout << "It seems that the test " << desc << " failed to write " << out_filename << " identical to tests/random.65536." << std::endl;
            passed = false;
        }
    }

    return passed;
}
// vim: set shiftwidth=4 softtabstop=4 :
