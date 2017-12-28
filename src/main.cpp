#include <cstring> // strcmp
#include <cstdlib> // strtol, EXIT_FAILURE, EXIT_SUCCESS, rand, srand
#include <fstream> // ofstream
#include <iostream>
#include <iomanip>
#include <utility>
//#include "memoryMapping.hpp"
#include "InputStream1.hpp"
#include "OutputStream1.hpp"
#include "InputStream2.hpp"
#include "OutputStream2.hpp"
#include "InputStream3.hpp"
#include "OutputStream3.hpp"

// #include "merge.cpp"
void noop();

void testStreams(AbstractOutputstream & os, AbstractInputstream & is, const char* const desc) {
    /* Test `os` and `is`, `desc` is a short description included in report messages
     * 1. Use `os` to write a file identical to the 'data' file (from hard-coded content)
     * 2. Use `is` to read the 'data' binary file (check with hard-coded content)
     * 3. Use `is` to read the data written by `os` (and check with the hard-coded content)
     *
     * The file written is named `desc` + ".out"
     */

    // reference
    const char DATA_FILENAME[]{"data"};
    const size_t DATA_SIZE(5);
    const int_least32_t DATA_CONTENT[DATA_SIZE]{97, 65, 16843009, 65537, -3};
    // out
    const char OUT_EXT[]{".out"};
    char * out_filename = new char[strlen(desc) + strlen(OUT_EXT) + 1];
    strcpy(out_filename, desc);
    strcat(out_filename, OUT_EXT);

    // in
    const size_t IN_FILES(2);
    const char * const IN_FILENAMES[2]{DATA_FILENAME, out_filename};

    // 1. Use `os` write a file identical to the 'data' file (from hard-coded content)
    std::cout << "BEGIN: Test output " << desc << " to   " << out_filename << "." << std::endl;
    /* This does not check the data written. Execute
     *     diff DATA_FILENAME out_filename
     * or
     *     vimdiff DATA_FILENAME out_filename
     * to check it (see makefile).
     */

    os.create(out_filename);
    for (size_t i = 0; i < DATA_SIZE; ++i) {
        os.write(DATA_CONTENT[i]);
    }
    os.close();

    std::cout << "END  : Test output " << desc << " to   " << out_filename << ".\n" << std::endl;

    // 2. Use `is` to read the 'data' binary file (check with hard-coded content)
    // 3. Use `is` to read the data written by `os` (and check with the hard-coded content)
    for (size_t f = 0; f < IN_FILES; ++f) {
        const char * const in_filename = IN_FILENAMES[f];
        std::cout << "BEGIN: Test input  " << desc << " from " << in_filename << "." << std::endl;

        is.open(in_filename);
        size_t i = 0;
        while (!is.end_of_stream() && i < DATA_SIZE) {
            int_least32_t read = is.read_next();
            if (DATA_CONTENT[i] != read) {
                std::cerr << "       " << i << ": "
                    << std::setw(10) << DATA_CONTENT[i] << " expected but " << std::setw(10) << read << " found."
                    << std::endl;
            }
            ++i;
        }
        if (i != DATA_SIZE) {
            std::cerr << "       Did not stop at the end (at " << i << " instead of " << DATA_SIZE << ")" << std::endl;
        }
        while (!is.end_of_stream()) {
            int_least32_t read = is.read_next();
            std::cerr << "       " << i << ": " << std::setw(10) << read << " found." << std::endl;
            ++i;
        }

        std::cout << "END  : Test input  " << desc << " from " << in_filename << ".\n" << std::endl;

        is.close();
    }
}

/** TODO */
void testMerge(size_t isc, AbstractInputstream * isv[], AbstractOutputstream & os) {
    char in_filename[9];
    for (size_t i = 0; i < isc; ++i) {
        snprintf(in_filename, 9, "sorted.%lu", i+1);
        isv[i]->open(in_filename);
    }
    os.create("merge.out");
    merge(isc, isv, os);
    for (size_t i = 0; i < isc; ++i) {
        isv[i]->close();
    }
    os.close();
}

int main(int argc, char* argv[]) {
    static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "Bad endian");

    if (argc < 2) {
        std::cerr << "Need argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (strcmp(argv[1], "1") == 0) {
        OutputStream1 os;
        InputStream1 is;
        testStreams(os, is, "test1");
    }
    else if (strcmp(argv[1], "2") == 0) {
        OutputStream2 os;
        InputStream2 is;
        testStreams(os, is, "test2");
    }
    else if (strcmp(argv[1], "3") == 0) {
        OutputStream3<4> os;
        InputStream3<4> is;
        testStreams(os, is, "test3");
    }
    /*else {
        // TESTING MEMORY_MAPPING, IMPLEMENTATION 4
        char *data;
        char* filename = argv[1];
        //MemoryMapping mmap = MemoryMapping(filename, 16);
        MemoryMapping mmap = MemoryMapping(filename);
        //data = mmap.writeFile();
        data = mmap.read_file();
    }
    */
    else if (strcmp(argv[1], "merge") == 0) {
        if (argc < 3) {
            std::cerr << "How many streams ?" << std::endl;
            exit(EXIT_FAILURE);
        }
        size_t isc(strtol(argv[2], nullptr, 10)); // Number of streams
        if (!(0 < isc && isc <= 3)) {
            std::cerr << "Only 1 to 3 streams supported to test for now (due to 3 sorted.x files only)" << std::endl;
            exit(EXIT_FAILURE);
        }
        AbstractInputstream ** isv = new AbstractInputstream*[isc];
        for (size_t i = 0; i < isc; ++i) {
            isv[i] = new InputStream1();
        }
        OutputStream1 os;
        testMerge(isc, isv, os);

        for (size_t i = 0; i < isc; ++i) {
            delete isv[i];
        }
    }

    return EXIT_SUCCESS;
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
