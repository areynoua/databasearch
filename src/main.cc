#include <cstring> // strcmp
#include <cstdlib> // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream> // cout
//#include <iomanip>
#include <unistd.h> // sysconf

#include <vector>

#include "InputStream1.hpp"
#include "OutputStream1.hpp"
#include "InputStream2.hpp"
#include "OutputStream2.hpp"
#include "InputStream3.hpp"
#include "OutputStream3.hpp"
#include "InputStream4.hpp"
#include "OutputStream4.hpp"
#include "streamTest.hpp"

#define streamarg(t,p) case p: for (size_t i = 0; i < number; ++i) streams[i] = new t<p>(); break;

AbstractInputstream ** InputStreamFactory(char * type, char * parameter, size_t number) {
    AbstractInputstream ** streams = new AbstractInputstream*[number];

    int t = atoi(type);
    int p = atoi(parameter);

    switch (t) {
        case 1:
            for (size_t i = 0; i < number; ++i) {
                streams[i] = new InputStream1();
            }
            break;
        case 2:
            for (size_t i = 0; i < number; ++i) {
                streams[i] = new InputStream2();
            }
            break;
        case 3:
            switch (p) {
                streamarg(InputStream3,2);
                streamarg(InputStream3,4);
                streamarg(InputStream3,8);
                streamarg(InputStream3,16);
                streamarg(InputStream3,32);
                streamarg(InputStream3,64);
                streamarg(InputStream3,128);
                streamarg(InputStream3,256);
                streamarg(InputStream3,512);
                streamarg(InputStream3,1024);
                streamarg(InputStream3,2048);
                streamarg(InputStream3,4096);
                streamarg(InputStream3,8192);
                streamarg(InputStream3,16384);
                streamarg(InputStream3,32768);
                default:
                    std::cout << "Choose 2, 4, 8, 16, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768 as buffer size" << endl;
                    exit(EXIT_FAILURE);
            }
            break;
        case 4:
            switch (p) {
                streamarg(InputStream4,1);
                streamarg(InputStream4,2);
                streamarg(InputStream4,4);
                streamarg(InputStream4,8);
                streamarg(InputStream4,16);
                streamarg(InputStream4,32);
                streamarg(InputStream4,64);
                streamarg(InputStream4,128);
                streamarg(InputStream4,256);
                streamarg(InputStream4,512);
                streamarg(InputStream4,1024);
                streamarg(InputStream4,2048);
                streamarg(InputStream4,4096);
                streamarg(InputStream4,8192);
                streamarg(InputStream4,16384);
                streamarg(InputStream4,32768);
                default:
                    std::cout << "Choose 1, 2, 4, 8, 16, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768 as number of page to map\n"
                        << "The page size is (bytes)" << sysconf(_SC_PAGESIZE) << std::endl;
                    exit(EXIT_FAILURE);
            }
            break;
        default:
            std::cout << "Choose 1, 2, 3 or 4 as stream implementation" << std::endl;
            exit(EXIT_FAILURE);
    }

    return streams;
}

AbstractOutputstream ** OutputStreamFactory(char * type, char * parameter, size_t number) {
    AbstractOutputstream ** streams = new AbstractOutputstream*[number];

    int t = atoi(type);
    int p = atoi(parameter);

    switch (t) {
        case 1:
            for (size_t i = 0; i < number; ++i) {
                streams[i] = new OutputStream1();
            }
            break;
        case 2:
            for (size_t i = 0; i < number; ++i) {
                streams[i] = new OutputStream2();
            }
            break;
        case 3:
            switch (p) {
                streamarg(OutputStream3,2);
                streamarg(OutputStream3,4);
                streamarg(OutputStream3,8);
                streamarg(OutputStream3,16);
                streamarg(OutputStream3,32);
                streamarg(OutputStream3,64);
                streamarg(OutputStream3,128);
                streamarg(OutputStream3,256);
                streamarg(OutputStream3,512);
                streamarg(OutputStream3,1024);
                streamarg(OutputStream3,2048);
                streamarg(OutputStream3,4096);
                streamarg(OutputStream3,8192);
                streamarg(OutputStream3,16384);
                streamarg(OutputStream3,32768);
                default:
                    std::cout << "Choose 2, 4, 8, 16, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768 as buffer size" << endl;
                    exit(EXIT_FAILURE);
            }
            break;
        case 4:
            switch (p) {
                streamarg(OutputStream4,1);
                streamarg(OutputStream4,2);
                streamarg(OutputStream4,4);
                streamarg(OutputStream4,8);
                streamarg(OutputStream4,16);
                streamarg(OutputStream4,32);
                streamarg(OutputStream4,64);
                streamarg(OutputStream4,128);
                streamarg(OutputStream4,256);
                streamarg(OutputStream4,512);
                streamarg(OutputStream4,1024);
                streamarg(OutputStream4,2048);
                streamarg(OutputStream4,4096);
                streamarg(OutputStream4,8192);
                streamarg(OutputStream4,16384);
                streamarg(OutputStream4,32768);
                default:
                    std::cout << "Choose 1, 2, 4, 8, 16, 256, 512, 1024, 2048, 4096, 8192, 16384 or 32768 as number of page to map\n"
                        << "The page size is (bytes)" << sysconf(_SC_PAGESIZE) << std::endl;
                    exit(EXIT_FAILURE);
            }
            break;
        default:
            std::cout << "Choose 1, 2, 3 or 4 as stream implementation" << std::endl;
            exit(EXIT_FAILURE);
    }

    return streams;
}


int main(int argc, char* argv[]) {
    static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "Bad endian");

    if (argc < 2) {
        std::cerr << "Need argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    // generate [nbFiles] [minQuantity] [maxQuantity]
    else if (strcmp(argv[1], "generate") == 0) {
        if (argc < 3) {
            std::cerr << argv[0] << " generate size_of_file_0 [ size_of_file_1 [ size_of_file_2 [ ... ] ] ]" << std::endl;
        }
        generate(argv);
    }

    // input <impl> [ <nbFiles> [ <impl-param> ] ]
    // <impl>: 1 | 2 | 3 | 4
    // <nbFile>: number. Defaults to max supported or 30 if 30 is smaller.
    // <impl-param>: the size of the impl3 buffer or the number of page mapped by impl4.
    //      must be a power of 2 (implementation is generic, but as the instanciation is static
    //      we restricted the possible values).
    //
    // with impl = 1 or impl = 2, impl-param is ignored
    // if impl = 3 or impl = 4, impl-param is required (and so is nbFiles)
    // ex:
    //  input 1
    //  input 1 0
    //  input 1 max
    //      read simultaneously the maximum number of file supported with impl 1
    //  input 2 3
    //      read simultaneously 3 files with impl 2
    //  input 3 - 256
    //      read simultaneously the maximum number of file supported with impl 3 with a buffer of 256 int32
    else if (strcmp(argv[1], "input") == 0){
        const int IMPL = 2;
        const int NB_FILES = 3;
        const int IMPL_PARAM = 4;

        size_t max_open_file(0);
        if (argc > NB_FILES) {
            max_open_file = atoi(argv[NB_FILES]);
        }
        if (max_open_file < 1 || 30 < max_open_file) {
            max_open_file = 30;
        }

        char * impl_param = new char[10];
        impl_param[0] = '\x0';
        if (argc > IMPL_PARAM) {
            impl_param = argv[IMPL_PARAM];
        }

        AbstractInputstream ** isVector;
        isVector = InputStreamFactory(argv[IMPL], impl_param, max_open_file);
        benchmarkInputStream(isVector, max_open_file);

        delete[] isVector;
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

        /*if (strcmp(argv[2], "4") == 0){
            for (int i = 0; i < max_open_file; ++i){
                OutputStream4* os = new OutputStream4(atoi(argv[6]));
                osVector.push_back(os);
            }
        }*/

        benchmarkOutputStream(osVector,atoi(argv[4]),atoi(argv[5]));
    }

    return EXIT_SUCCESS;
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
