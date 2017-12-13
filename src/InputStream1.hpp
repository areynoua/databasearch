#ifndef DEF_INPUTSTREAM1
#define DEF_INPUTSTREAM1

#include <cstdint>
#include <error.h>
#include <fcntl.h>
#include <unistd.h>

#include "common.hpp"

using namespace std;

class InputStream1 final : virtual public AbstractInputstream {
    static const size_t SIZE = 32 / 8; // 32 bits
    int fd;
    char buffer[SIZE];
    ssize_t read_size;

public:
    InputStream1(): fd(-1), read_size(0) {}
    virtual ~InputStream1();

    void open(const char* const) override;
    int_least32_t read_next() override;
    inline bool end_of_stream() override;
    void close() override;
};

InputStream1::~InputStream1() {
    close();
}

void InputStream1::open(const char* const filename) {
    fd = ::open(filename, O_RDONLY | O_LARGEFILE);
    if (fd < 0) {
        throw FileOpenException(errno);
    }
    read_next(); // fill the buffer
}

int_least32_t InputStream1::read_next() {
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

    read_size = read(fd, buffer, SIZE);

    return value;
}

bool InputStream1::end_of_stream() {
    return read_size == 0;
}

void InputStream1::close() {
    if (fd != -1) {
        if (::close(fd) == 0) {
            fd = -1;
        }
        else {
            throw FileCloseException(errno);
        }
    }
}

#endif

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
