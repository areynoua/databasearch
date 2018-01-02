#include <unistd.h>
#include <iostream>
#include <vector>
#include "common.hpp"
#include "OutputStream1.hpp"
using namespace std;




/**
 * Use std::rand to return a pseudo random number between -2^(31) and 2^(31)-1.
 * note: std::rand return a value between 0 and RAND_MAX with RAND_MAX >= 2^15 - 1
 */
inline int_least32_t rand32() {
    char chars[4];
    chars[0] = static_cast<char>(std::rand());
    chars[1] = static_cast<char>(std::rand());
    chars[2] = static_cast<char>(std::rand());
    chars[3] = static_cast<char>(std::rand());
    return charsToInt32(chars);
}


/**
 * Generate convenient files with int32 values.
 * Each string in argv[2:] is interpreted as the size of a file.
 * For example with argc = 5 and argv = {"./streams", "generate", "4", "4", "12"}
 * this will produce data.1 with 4 numbers, data.2 with 4 numbers and data.3 with 12 numbers.
 * Each time files are generated, they have the same numbers as long as the execution
 * is in the same environment.
 */
void generate(char ** argv) {
    OutputStream1 os;
//    std::ofstream otxts;
    char filename[26];
    char txtfilename[29];
    size_t min = strtoll(argv[3], nullptr, 10);
    size_t max = strtoll(argv[4], nullptr, 10);
    long max_open_file;
    if (strcmp(argv[2],"max") == 0){
        max_open_file = sysconf(_SC_OPEN_MAX);
    }
    else{
        max_open_file = atoi(argv[2]);
    }
    for (int f = 1; f < max_open_file+1; ++f) {

        std::srand(static_cast<unsigned>(f));
        size_t size = rand()%(max-min) + min;
        int success = snprintf(filename, 26, "data files/data.%d", f-1);
        snprintf(txtfilename, 29, "data files/data.%d.txt", f-1);
        if (success < 5 || success > 24 || size < 1) {
            std::cerr << "Bad file size" << std::endl;
            continue;
        }
        os.create(filename);
//        otxts.open(txtfilename, ios_base::trunc | ios_base::out);
        for (size_t i = 0; i < size; ++i) {
            int_least32_t value = rand32();
            os.write(value);
            //otxts << value << "\n";
        }
//        otxts << std::flush;
//        otxts.close();
        os.close();
    }
}


/**
 * /!\ this method required that end_of_stream() is false for all the streams.
 * /!\ this method delete all the streams in isVector, but not the array itself.
 */
void benchmarkInputStream(AbstractInputstream ** isVector, size_t vectorSize){
    // We open our K stream based on the quantity of inputstreams
    for (size_t i = 0; i < vectorSize; ++i){
        AbstractInputstream* is = isVector[i];
        char DATA_FILENAME[]{"data files/data."};
        char str[10];
        sprintf(str, "%ld", i);
        is->open(strcat(DATA_FILENAME,str));
    }

    while (vectorSize > 0){
        for (size_t i = 0; i < vectorSize; ++i){
            AbstractInputstream* is = isVector[i];
            is->read_next();

            if (is->end_of_stream()){
                --vectorSize;
                std::swap(isVector[i], isVector[vectorSize]);
                is->close();
                delete is;
                --i;
            }
        }
    }
}

/**
 * /!\ this method required that end_of_stream() is false for all the streams.
 * /!\ this method delete all the streams in isVector, but not the array itself.
 */
void benchmarkOutputStream(AbstractOutputstream ** osVector, size_t vectorSize, size_t filesSize) {
    // We open our K stream based on the quantity of inputstreams
    for (size_t i = 0; i < vectorSize; ++i){
        AbstractOutputstream* os = osVector[i];
        char DATA_FILENAME[]{"output data/od."};
        char str[10];
        sprintf(str, "%ld", i);
        os->create(strcat(DATA_FILENAME,str));
    }
    for (size_t j = 0; j < filesSize; ++j) {
        for (size_t i = 0; i < vectorSize; ++i){
            AbstractOutputstream* os = osVector[i];
            os->write(rand32());
        }
    }
    for (size_t i = 0; i < vectorSize; ++i){
        AbstractOutputstream* os = osVector[i];
        os->close();
        delete os;
    }
}
// vim: set shiftwidth=4 softtabstop=4 :
