#ifndef DEF_INPUTSTREAM3
#define DEF_INPUTSTREAM3

#include <cstdint> // int_least32_t
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <unistd.h>

#include "common.hpp"

// using int32_filebuf = std::basic_filebuf<std::int_least32_t, std::char_traits<char32_t>>;
using string=std::string;

template <size_t _bufferSize>
class InputStream3 final : virtual public AbstractInputstream {
    char _buffer[_bufferSize*4];
    int _fd = -1;
    size_t _next = 0;
    size_t _max = 0;
    bool _eof = false;
public:
    InputStream3(){}
    virtual ~InputStream3();
    void open(const char* const) override;
    int_least32_t read_next() override;
    bool end_of_stream() override;
    void close() override;
private:
    void fillBuffer();
};

template <size_t _bufferSize>
InputStream3<_bufferSize>::~InputStream3() {
    close();
}

template <size_t _bufferSize>
void InputStream3<_bufferSize>::open(const char* const pathname) {
    _fd = ::open(pathname, O_RDONLY | O_LARGEFILE);
    // TODO: throw error if fd < 0
    std::cout << pathname << std::endl;
}

template <size_t _bufferSize>
int_least32_t InputStream3<_bufferSize>::read_next() {
    if (_next == _max) {
        _max = read(_fd, _buffer, _bufferSize*4);
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
bool InputStream3<_bufferSize>::end_of_stream() {
    // FIXME
    return _eof;
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
