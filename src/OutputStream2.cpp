#include <unistd.h>
#include <iostream>

#include "OutputStream2.hpp"

OutputStream2::~OutputStream2() {
    close();
    // delete file_pointer;
}

void OutputStream2::create(const char* const filename) {
    close();
    file_pointer = fopen(filename, "w");
    if (file_pointer == nullptr) {
        throw FileOpenException(errno);
    }
}

void OutputStream2::write(int_least32_t number) {
    ssize_t written_size(fwrite(&number, 4, 1, file_pointer));

    if (written_size != 1) {
        if (ferror(file_pointer)) {
            throw FileWriteException(EIO);
        }
    }
}

void OutputStream2::close() {
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
