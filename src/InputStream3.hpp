#ifndef DEF_INPUTSTREAM3
#define DEF_INPUTSTREAM3

#include <cstdint> // int_least32_t
#include <error.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <unistd.h>

#include "common.hpp"

template <size_t _bufferSize> /* the number of int32 */
class InputStream3 final : virtual public AbstractInputstream {
    static const size_t SIZE = 32 / 8; /* the number of bytes in 1 int32 */
    char _buffer[_bufferSize*SIZE];
    int _fd = -1;
    size_t _next = 0; /* in bytes */
    size_t _read_size = 0; /* in bytes */
public:
    InputStream3(){}
    virtual ~InputStream3();
    void open(const char* const) override;
    int_least32_t read_next() override;
    bool end_of_stream() override;
    void close() override;
};

template <size_t _bufferSize>
InputStream3<_bufferSize>::~InputStream3() {
    close();
}

template <size_t _bufferSize>
void InputStream3<_bufferSize>::open(const char* const pathname) {
    close();
    _fd = ::open(pathname, O_RDONLY | O_LARGEFILE);
    if (_fd < 0) {
        throw FileOpenException(errno);
    }
    read_next(); // fill the buffer
}

template <size_t _bufferSize>
int_least32_t InputStream3<_bufferSize>::read_next() {
    int_least32_t value(0);

    if (_next + SIZE <= _read_size) { // OK: enough bytes to read
        value = charsToInt32(&(_buffer[_next]));
        _next += SIZE;
    }
    else {
        if (_read_size > 0) {
            error(0, 0, "Last %ld bytes ignored (%ld bytes expected)", _read_size, SIZE);
        }
        else if (_read_size < 0) {
            throw FileReadException(errno);
        }
        // (_read_size == 0) { first call (from constructor) or EOF }
    }

    if (_next == _read_size) {
        _read_size = read(_fd, _buffer, _bufferSize*SIZE);
        if (_read_size < 0) {
            throw FileReadException(errno);
        }
        _next = 0;
    }

    return value;
}

template <size_t _bufferSize>
bool InputStream3<_bufferSize>::end_of_stream() {
    return _read_size == 0;
}
template <size_t _bufferSize>
void InputStream3<_bufferSize>::close() {
    if (_fd != -1) {
        if (::close(_fd) == 0) {
            _fd = -1;
        }
        else {
            throw FileCloseException(errno);
        }
    }
}

#endif // __IMPL3_H__

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
