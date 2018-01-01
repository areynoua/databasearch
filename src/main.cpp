#include <cstring> // strcmp
#include <cstdlib> // strtol, EXIT_FAILURE, EXIT_SUCCESS, rand, srand
#include <fstream> // ofstream
#include <iostream>
#include <iomanip>
#include <utility>

#include <unistd.h>

#include <vector>

#include "InputStream1.hpp"
#include "OutputStream1.hpp"
#include "InputStream2.hpp"
#include "OutputStream2.hpp"
#include "InputStream3.hpp"
#include "OutputStream3.hpp"
#include "InputStream4.hpp"
#include "OutputStream4.hpp"
#include "merge.hpp"
#include "streamTest.hpp"



/**
 * Test `os` and `is`, `desc` is a short description included in report messages
 * 1. Use `os` to write a file identical to the 'data' file (from hard-coded content)
 * 2. Use `is` to read the 'data' binary file (check with hard-coded content)
 * 3. Use `is` to read the data written by `os` (and check with the hard-coded content)
 * 4. Use `os` and `is` to create a copy of "random.65536"
 *
 * The file written is named `desc` + ".out"
 */
void testStreams(AbstractOutputstream & os, AbstractInputstream & is, const char* const desc) {
    // reference
    const char DATA_FILENAME[]{"data files/data"};
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

    // 4. open on another file
    is.open("random.65536");
    os.create("random.cpy");
    while (! is.end_of_stream()) {
        os.write(is.read_next());
    }
    os.close();
    is.close();
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
    else if (strcmp(argv[1], "4") == 0) {
        OutputStream4 os(4);
        InputStream4 is(4);
        testStreams(os, is, "test4");
    }
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
    // generate [nbFiles] [minQuantity] [maxQuantity]
    else if (strcmp(argv[1], "generate") == 0) {
        if (argc < 3) {
            std::cerr << argv[0] << " generate size_of_file_0 [ size_of_file_1 [ size_of_file_2 [ ... ] ] ]" << std::endl;
        }
        generate(argv);
    }

    // input [1-4] [nbFiles]
    else if (strcmp(argv[1], "input") == 0){
        long max_open_file;
        if (strcmp(argv[3],"max") == 0){
            max_open_file = sysconf(_SC_OPEN_MAX);
        }
        else{
            max_open_file = atoi(argv[3]);
        }
        vector<AbstractInputstream*> isVector;
        if (strcmp(argv[2], "1") == 0){
            for (int i = 0; i < max_open_file; ++i){
                InputStream1* is = new InputStream1();
                isVector.push_back(is);
            }
        }
        else if (strcmp(argv[2], "2") == 0){
            for (int i = 0; i < max_open_file; ++i){
                InputStream2* is = new InputStream2();
                isVector.push_back(is);
            }
        }
        else if (strcmp(argv[2], "3") == 0){
            size_t bufferSize = atoi(argv[4]);
            for (int i = 0; i < max_open_file; ++i){
                if (bufferSize == 2) {
                    InputStream3<2>* is = new InputStream3<2>();
                    isVector.push_back(is);
                }
                else if (bufferSize == 4) {
                    InputStream3<4>* is = new InputStream3<4>();
                    isVector.push_back(is);
                }
                else if (bufferSize == 8) {
                    InputStream3<8>* is = new InputStream3<8>();
                    isVector.push_back(is);
                }
                else if (bufferSize == 16) {
                    InputStream3<16>* is = new InputStream3<16>();
                    isVector.push_back(is);
                }
                else if (bufferSize == 32) {
                    InputStream3<32>* is = new InputStream3<32>();
                    isVector.push_back(is);
                }
                else if (bufferSize == 256) {
                    InputStream3<256>* is = new InputStream3<256>();
                    isVector.push_back(is);
                }
                else if (bufferSize == 512) {
                    InputStream3<512>* is = new InputStream3<512>();
                    isVector.push_back(is);
                }
                else if (bufferSize == 1024) {
                    InputStream3<1024>* is = new InputStream3<1024>();
                    isVector.push_back(is);
                }
                else if (bufferSize == 2048) {
                    InputStream3<2048>* is = new InputStream3<2048>();
                    isVector.push_back(is);
                }
                else if (bufferSize == 4096) {
                    InputStream3<4096>* is = new InputStream3<4096>();
                    isVector.push_back(is);
                }
                else {
                    cout << "choose 2, 4, 8, 16, 256, 512, 1024, 2048 or 4096" << endl;
                }
            }
        }
        else if (strcmp(argv[2], "4") == 0){
            for (int i = 0; i < max_open_file; ++i){
                InputStream4* is = new InputStream4(atoi(argv[4]));
                isVector.push_back(is);
            }
        }
        benchmarkInputStream(isVector);
    }
    // output [1-4 ][nbFiles] [minQuantity] [maxQuantity]
    else if (strcmp(argv[1],"output") == 0){
        long max_open_file;
        if (strcmp(argv[3],"max") == 0){
            max_open_file = sysconf(_SC_OPEN_MAX);
        }
        else{
            max_open_file = atoi(argv[3]);
        }
        vector<AbstractOutputstream*> osVector;
        if (strcmp(argv[2], "1") == 0){
            for (int i = 0; i < max_open_file; ++i){
                OutputStream1* os = new OutputStream1();
                osVector.push_back(os);
            }
        }
        if (strcmp(argv[2], "2") == 0){
            for (int i = 0; i < max_open_file; ++i){
                OutputStream2* os = new OutputStream2();
                osVector.push_back(os);
            }
        }

        else if (strcmp(argv[2], "3") == 0){
            size_t bufferSize = atoi(argv[6]);
            for (int i = 0; i < max_open_file; ++i){
                if (bufferSize == 2) {
                    OutputStream3<2>* os = new OutputStream3<2>();
                    osVector.push_back(os);
                }
                else if (bufferSize == 4) {
                    OutputStream3<4>* os = new OutputStream3<4>();
                    osVector.push_back(os);
                }
                else if (bufferSize == 8) {
                    OutputStream3<8>* os = new OutputStream3<8>();
                    osVector.push_back(os);
                }
                else if (bufferSize == 16) {
                    OutputStream3<16>* os = new OutputStream3<16>();
                    osVector.push_back(os);
                }
                else if (bufferSize == 32) {
                    OutputStream3<32>* os = new OutputStream3<32>();
                    osVector.push_back(os);
                }
                else if (bufferSize == 256) {
                    OutputStream3<256>* os = new OutputStream3<256>();
                    osVector.push_back(os);
                }
                else if (bufferSize == 512) {
                    OutputStream3<512>* os = new OutputStream3<512>();
                    osVector.push_back(os);
                }
                else if (bufferSize == 1024) {
                    OutputStream3<1024>* os = new OutputStream3<1024>();
                    osVector.push_back(os);
                }
                else if (bufferSize == 2048) {
                    OutputStream3<2048>* os = new OutputStream3<2048>();
                    osVector.push_back(os);
                }
                else if (bufferSize == 4096) {
                    OutputStream3<4096>* os = new OutputStream3<4096>();
                    osVector.push_back(os);
                }
                else {
                    cout << "choose 2, 4, 8, 16, 256, 512, 1024, 2048 or 4096" << endl;
                }
            }
        }

        if (strcmp(argv[2], "4") == 0){
            for (int i = 0; i < max_open_file; ++i){
                OutputStream4* os = new OutputStream4(atoi(argv[6]));
                osVector.push_back(os);
            }
        }

        benchmarkOutputStream(osVector,atoi(argv[4]),atoi(argv[5]));
    }

    return EXIT_SUCCESS;
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
