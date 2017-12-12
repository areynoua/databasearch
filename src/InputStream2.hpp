#ifndef DEF_INPUTSTREAM2
#define DEF_INPUTSTREAM2

#include <iostream>
#include <cstdint>

#include "common.hpp"

using namespace std;

class InputStream2 final : virtual public AbstractInputstream {
private:
    static const size_t SIZE = 32 / 8; // 32 bits
    FILE *file_pointer;
    char buffer[SIZE];
    size_t read_size;
public:
    InputStream2(): file_pointer(nullptr), read_size(0){}
    void open(const char* const) override;
    int_least32_t read_next() override;
    bool end_of_stream() override;
};


void InputStream2::open(const char* filename){
    file_pointer = fopen(filename, "r");
    if (file_pointer == nullptr) {
        throw FileOpenException(errno);
    }
    fseek(file_pointer, 0, SEEK_SET); // TODO: is it really needed ?
    read_next(); // fill the buffer
}

int_least32_t InputStream2::read_next(){
    /* Return the value in the buffer when the function is called
     * and fill the buffer for the next value.
     * This allow to handle end_of_stream conveniently. */

    int_least32_t value(0);

    if (read_size == SIZE) {
        value = charsToInt32(buffer);
    }
    else {
        if (read_size > 0) {
            error(0, 0, "Last %ld bytes ignored (4 bytes expected)", read_size);
        }
        else if (read_size < 0) {
            throw FileReadException(errno);
        }
    }

    // FIXME: fread does not work this way
    read_size = fread(buffer, SIZE, 1, file_pointer);

    return value;
}

bool InputStream2::end_of_stream(){
    return read_size == 0;
}

#endif

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
