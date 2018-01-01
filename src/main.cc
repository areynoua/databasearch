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

        benchmarkOutputStream(osVector,atoi(argv[4]),atoi(argv[5]));
    }

    return EXIT_SUCCESS;
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
