#ifndef DEF_INPUTSTREAM2
#define DEF_INPUTSTREAM2

#include <iostream>
#include <cstdint>
#include <error.h>

#include "common.hpp"

using namespace std;

class InputStream2 final : virtual public AbstractInputstream {
private:
    static const size_t SIZE = 32 / 8; // 32 bits
    FILE *file_pointer;
    int_least32_t read_value;
    ssize_t read_size;

public:
    InputStream2()
        : file_pointer(nullptr), read_value(0), read_size(0) {}
    InputStream2(const InputStream2&) = delete;
    InputStream2& operator=(const InputStream2&) = delete;
    virtual ~InputStream2();

    void open(const char* const) override;
    int_least32_t read_next() override;
    bool end_of_stream() override;
    void close() override;
};

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

#endif

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
