#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <cstdint>

// Exceptions

#include <exception>
#include <cstring>
#include <cerrno>

class FileOpenException : public std::exception {
    int _errno;
public:
    virtual ~FileOpenException() = default;

    FileOpenException(int const errcode) : _errno(errcode) {}
    virtual const char* what() const noexcept { return std::strerror(_errno); }
};

class FileReadException : public std::exception {
    int _errno;
public:
    virtual ~FileReadException() = default;

    FileReadException(int const errcode) : _errno(errcode) {}
    virtual const char* what() const noexcept { return std::strerror(_errno); }
};

// Bases

class AbstractInputstream {
public:
    virtual ~AbstractInputstream() = default;

    virtual void open(const char* const infilename) = 0;
    virtual int_least32_t read_next() = 0;
    virtual bool end_of_stream() = 0;
};

class AbstractOutputstream {
public:
    virtual ~AbstractOutputstream() = default;

    virtual void create(const char* const) = 0;
    virtual void write_file (int_least32_t elem) = 0;
    virtual void close() = 0;
};

// Utils

int_least32_t charsToInt32 (const char* const chars) {
    return (static_cast<int_least32_t>(chars[0]) << 24)
        | (static_cast<int_least32_t>(chars[1]) << 16)
        | (static_cast<int_least32_t>(chars[2]) << 8)
        | (static_cast<int_least32_t>(chars[3]));
}

#include <iostream>

void print_all(AbstractInputstream& is) {
    while (!is.end_of_stream()) {
        std::cout << is.read_next() << std::endl;;
    }
}

#endif //__COMMON_HPP__

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
