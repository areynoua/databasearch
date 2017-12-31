#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <cstdint>
#include <cstddef>

// Exceptions

#include <exception>
#include <cstring>
#include <cerrno>
#include <string>

class FileException : public std::exception {
protected:
    int _errno;
    std::string _msg;

public:
    virtual ~FileException() = default;

    FileException(int const errcode, std::string msg = std::string()) : _errno(errcode), _msg(msg) {}

    virtual const char* what() const noexcept {
        std::string msg(std::strerror(_errno));
        if (! _msg.empty()) {
            msg.push_back('\n');
            msg.append(_msg);
        }
        return msg.c_str();
    }
};

class FileOpenException : public FileException {
    using FileException::FileException;
};

class FileReadException : public FileException {
    using FileException::FileException;
};

class FileWriteException : public FileException {
    using FileException::FileException;
};

class FileCloseException : public FileException {
    using FileException::FileException;
};


// Bases

class AbstractInputstream {
public:
    virtual ~AbstractInputstream() = default;

    virtual void open(const char* const infilename) = 0;
    virtual int_least32_t read_next() = 0;
    virtual bool end_of_stream() = 0;

    virtual void close() = 0;
};

class AbstractOutputstream {
public:
    virtual ~AbstractOutputstream() = default;

    virtual void create(const char* const) = 0;
    virtual void write (int_least32_t elem) = 0;
    virtual void close() = 0;
};

// Utils

#include <cstring>

inline int_least32_t charsToInt32 (const char* const chars) {
    return *(reinterpret_cast<const int32_t* const>(chars));
}

inline void int32ToChars (char dest[4], int_least32_t number) {
    std::memcpy(dest, reinterpret_cast<char*>(&number), 4);
}

void print_all(AbstractInputstream& is);

#endif //__COMMON_HPP__

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
