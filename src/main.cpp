#include <cstring> // strcmp
#include <iostream>
#include <iomanip>
#include <utility>
//#include "memoryMapping.hpp"
#include "InputStream1.hpp"
#include "OutputStream1.hpp"
#include "InputStream2.hpp"
#include "OutputStream2.hpp"
//#include "InputStream3.hpp"


void noop();

void testStreams(AbstractOutputstream & os, AbstractInputstream & is, const char* const desc) {
    /* Test `os` and `is`, `desc` is a short description included in report messages
     * 1. Use `os` write a file identical to the 'data' file (from hard-coded content)
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
     * to check it.
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

int main(int argc, char* argv[]) {

    static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "Bad endian");
    if (argc < 2) {
        std::cerr << "Need argument" << std::endl;
    }
    if (strcmp(argv[1], "nothing") == 0) {
        noop();
        std::cout << "OK. Nothing done." << std::endl;
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
    /*
    else if (strcmp(argv[1], "buffered") == 0) {
        BufferedInputstream<2> bis = BufferedInputstream<2>();
        bis.open("random.16");
        for (int i = 0; i < 4; ++i) {
            std::cout << bis.read_next() << std::endl;
        }
        std::cout << bis.end_of_stream() << std::endl;
    }
    else {
        // TESTING MEMORY_MAPPING, IMPLEMENTATION 4
        char *data;
        char* filename = argv[1];
        //MemoryMapping mmap = MemoryMapping(filename, 16);
        MemoryMapping mmap = MemoryMapping(filename);
        //data = mmap.writeFile();
        data = mmap.read_file();
    }
    */

        return 0;
}

void noop() {
    // no operation
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
