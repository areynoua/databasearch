#ifndef DEF_OUTPUTSTREAM3
#define DEF_OUTPUTSTREAM3

#include <utility>

#include "common.hpp"

using namespace std;

template <size_t _bufferSize>
class OutputStream3 final : virtual public AbstractOutputstream {
    static const size_t SIZE = 32 / 8;
    char _buffer[_bufferSize*SIZE];
    int _fd = -1;
    size_t _next = 0;
public:
    OutputStream3(){}
    virtual ~OutputStream3();

    void create(const char* const) override;
    void write(int_least32_t elem) override;
    void close() override;
};

#include <error.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

template <size_t _bufferSize>
OutputStream3<_bufferSize>::~OutputStream3() {
    close();
}


template <size_t _bufferSize>
void OutputStream3<_bufferSize>::create(const char* const filename) {
    close();
    _fd = ::open(filename, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    // TODO: we should try with and without the O_DIRECT flag
    //       (described in open(2)).
    if (_fd < 0) {
        throw FileOpenException(errno);
    }
}


template <size_t _bufferSize>
void OutputStream3<_bufferSize>::write(int_least32_t number) {
    int32ToChars(&(_buffer[_next]), std::move(number));
    _next += SIZE;
    if (_next == _bufferSize*SIZE) {
        ssize_t written_size = ::write(_fd,_buffer,_bufferSize*SIZE);
        _next = 0;
        if (written_size != _bufferSize*SIZE) {
            if (written_size >= 0) {
                error(0, 0, "Write interrupted after %ld bytes", written_size);
            }
            else {
                throw FileWriteException(errno);
            }
        }
    }
}

template <size_t _bufferSize>
void OutputStream3<_bufferSize>::close() {
    if (_fd != -1) {
        if (_next != 0){
            ::write(_fd,_buffer,_next);
        }
        if (::close(_fd) == 0) {
            _fd = -1;
        }
        else {
            throw FileCloseException(errno);
        }
    }
}

#endif

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
