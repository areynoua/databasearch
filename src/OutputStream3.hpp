#ifndef DEF_OUTPUTSTREAM3
#define DEF_OUTPUTSTREAM3

#include <error.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#include "common.hpp"

using namespace std;

template <size_t _bufferSize>
class OutputStream3 final : virtual public AbstractOutputstream {
private:
    char _buffer[_bufferSize*4];
    int _fd = -1;
    size_t _next = 0;
public:
    OutputStream3(){}
    virtual ~OutputStream3();

    void create(const char* const) override;
    void write(int_least32_t elem) override;
    void close() override;
private:
    void int32ToChars(char*, const int_least32_t&, int);

};

template <size_t _bufferSize>
OutputStream3<_bufferSize>::~OutputStream3() {
    close();
}


template <size_t _bufferSize>
void OutputStream3<_bufferSize>::create(const char* const filename) {
    _fd = ::open(filename, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    // TODO: we should try with and without the O_DIRECT flag
    //       (described in open(2)).
    if (_fd < 0) {
        throw FileOpenException(errno);
    }
}


template <size_t _bufferSize>
void OutputStream3<_bufferSize>::write(int number) {
    int32ToChars(_buffer,number, _next);
    _next += 4;
    if (_next == _bufferSize*4) {
        ssize_t written_size = ::write(_fd,_buffer,_bufferSize*4);
        _next = 0;
        if (written_size != _bufferSize*4) {
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



//Temporary ?
template <size_t _bufferSize>
void OutputStream3<_bufferSize>::int32ToChars (char dest[], const int_least32_t& number, int index) {
    dest[index] = static_cast<char>(number);
    dest[index+1] = static_cast<char>(number >> 8);
    dest[index+2] = static_cast<char>(number >> 16);
    dest[index+3] = static_cast<char>(number >> 24);
}


#endif
// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
