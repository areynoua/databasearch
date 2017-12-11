#ifndef __IMPL3_H__
#define __IMPL3_H__

#include <cstdint> // int_least32_t
#include <fcntl.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>


#include <iostream>

#include "streams.hpp"

// using int32_filebuf = std::basic_filebuf<std::int_least32_t, std::char_traits<char32_t>>;
using string=std::string;

template <size_t _bufferSize>
class BufferedInputstream  {
    char _buffer[_bufferSize];
    int _fd = -1;
    size_t _next = 0;
    size_t _max = 0;
    bool _eof = false;
public:
    BufferedInputstream();
    void open_file(const char*);
    int_least32_t read_next();
    bool end_of_stream();
private:
    inline int_least32_t charsToInt32(char* chars);
    void fillBuffer();
};

template <size_t _bufferSize>
BufferedInputstream<_bufferSize>::BufferedInputstream() {

}

template <size_t _bufferSize>
void BufferedInputstream<_bufferSize>::open_file(const char *pathname) {
    _fd = open(pathname, O_RDONLY | O_LARGEFILE);
    // TODO: throw error if fd < 0
    std::cout << pathname << std::endl;
}

template <size_t _bufferSize>
int_least32_t BufferedInputstream<_bufferSize>::read_next() {
    if (_next == _max) {
        _max = read(_fd, _buffer, _bufferSize);
        std::cout << _max << " bytes read from file" << std::endl;
        if (_max == 0) {
            _eof = true;
        }
        // TODO: if max == 0: EOF
        // TODO: if max < 0: ERROR
        // TODO: not multiple of 4 bytes
        _next = 0;
    }
    int_least32_t ret = charsToInt32(&(_buffer[_next]));
    _next += 4;
    return ret;
}

template <size_t _bufferSize>
bool BufferedInputstream<_bufferSize>::end_of_stream() {
    return _eof;
}

template <size_t _bufferSize>
int_least32_t BufferedInputstream<_bufferSize>::charsToInt32 (char* chars) {
    return (static_cast<int_least32_t>(chars[0]) << 24)
        | (static_cast<int_least32_t>(chars[1]) << 16)
        | (static_cast<int_least32_t>(chars[2]) << 8)
        | (static_cast<int_least32_t>(chars[3]));
}

#endif // __IMPL3_H__

// vim: set shiftwidth=4:
