#include <cstdint>
#include <error.h>
#include <iostream>

#include "InputStream2.hpp"

InputStream2::~InputStream2() {
    close();
    delete file_pointer;
}

void InputStream2::open(const char* const filename) {
    file_pointer = fopen(filename, "r");
    if (file_pointer == nullptr) {
        throw FileOpenException(errno);
    }
    // fill read_value
    read_next();
}

int_least32_t InputStream2::read_next() {
    /* Return the last value read
     * and fill read_value with the next value.
     * This allow to handle end_of_stream conveniently. */

    int_least32_t value(read_value);

    if (read_size != 1) {
        if (ferror(file_pointer)) {
            throw FileReadException(EIO);
        }
    }

    read_size = fread(&read_value, SIZE, 1, file_pointer);

    return value;
}

bool InputStream2::end_of_stream() {
    return feof(file_pointer);
}

void InputStream2::close() {
    if (file_pointer != nullptr) {
        if (fclose(file_pointer) == 0) {
            file_pointer = nullptr;
        }
        else {
            throw FileCloseException(errno);
        }
    }
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
