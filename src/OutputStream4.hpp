#ifndef DEF_OUTPUTSTREAM4
#define DEF_OUTPUTSTREAM4

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <utility>
#include <sys/mman.h>

#include "common.hpp"

// TODO use madvise (linux; maybe not unix)
// TODO: check that the size accepted by the system

template <int PAGEC> // number of page to map
class OutputStream4 final : virtual public AbstractOutputstream {
    static const size_t PAGESIZE; // The page size in bytes
    static const size_t SIZE; // The number of bytes in 1 int32
    static const size_t MAPLEN; // The size of the mapping (in bytes)
    int _fd = -1; // File descriptor
    char* _map = nullptr; // The address of the mapping
    size_t _fileSize = 0; // The size of the mapped file (in bytes)
    size_t _next = 0; // The offset of the next bytes to read according to the address of the mapping

public:
    OutputStream4() {};
    OutputStream4(const OutputStream4&) = delete;
    OutputStream4& operator=(const OutputStream4&) = delete;
    virtual ~OutputStream4();

    void create(const char* const) override;
    void write(int_least32_t) override;
    void close() override;

private:
    void mapNext();
    void unmap();
};

template <int PAGEC>
const size_t OutputStream4<PAGEC>::PAGESIZE = sysconf(_SC_PAGESIZE);
template <int PAGEC>
const size_t OutputStream4<PAGEC>::SIZE = 32 / 8; // the number of bytes in 1 int32
template <int PAGEC>
const size_t OutputStream4<PAGEC>::MAPLEN = PAGEC*PAGESIZE; // The size of the mapping (in bytes)

template <int PAGEC>
OutputStream4<PAGEC>::~OutputStream4() {
    close();
}

template <int PAGEC>
void OutputStream4<PAGEC>::create(const char* const pathname) {
    close();
    _fd = ::open(pathname, O_RDWR|O_CREAT|O_TRUNC, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
    // TODO: we should try with and without the O_DIRECT flag
    //       (described in open(2)).

    if (_fd < 0) {
        throw FileOpenException(errno, "::open failed");
    }

    mapNext();
}

template <int PAGEC>
void OutputStream4<PAGEC>::write(int_least32_t number) {
    if (_next + SIZE > MAPLEN) {
        mapNext();
    }
    int32ToChars(&(_map[_next]), std::move(number));
    _next += SIZE;
}

template <int PAGEC>
void OutputStream4<PAGEC>::close() {
    size_t fileSize(_fileSize);
    size_t next(_next);
    unmap();

    if (_fd != -1) {
        if (ftruncate(_fd, fileSize + next - MAPLEN) != 0) {
            throw FileCloseException(errno, "ftruncate failed");
        }

        if (::close(_fd) == 0) {
            _fd = -1;
            _fileSize = 0;
        }
        else {
            throw FileCloseException(errno, "::close failed");
        }
    }
}

template <int PAGEC>
void OutputStream4<PAGEC>::mapNext() {
    char* oldMap(_map);
    unmap();

    // Increase the file size
    if (lseek(_fd, MAPLEN, SEEK_CUR) < 0) {
        char message[30];
        snprintf(message, 29, "lseek to %ld (%ld + %ld - 1) failed", _fileSize+MAPLEN-1, _fileSize, MAPLEN);
        throw FileWriteException(errno, message);
    }
    if (::write(_fd, "", 1) < 1) {
        throw FileWriteException(errno, "::write failed");
    }

    void *map = mmap(oldMap, MAPLEN, PROT_WRITE, MAP_SHARED, _fd, _fileSize);
    if (map == MAP_FAILED) {
        throw FileOpenException(errno, "mmap failed");
    }
    _fileSize += MAPLEN;
    _map = reinterpret_cast<char*>(map);
    _next = 0;
}

template <int PAGEC>
void OutputStream4<PAGEC>::unmap() {
    if (_map != nullptr) {
        if (munmap(_map, MAPLEN) == 0) {
            _map = nullptr;
            _next = 0;
        }
        else {
            throw FileCloseException(errno, "unmap failed");
        }
    }
}
#endif // DEF_OUTPUTSTREAM4
// vim: set shiftwidth=4 softtabstop=4 :
