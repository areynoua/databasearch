#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <utility>
#include <sys/mman.h>

#include "OutputStream4.hpp"

// TODO use madvise (linux; maybe not unix)

const size_t OutputStream4::_pageSize = sysconf(_SC_PAGESIZE); // The page size in bytes

OutputStream4::OutputStream4(size_t mapLen) {
    // TODO: check that the size accepted by the system

    // compute _mapLen as the lowest integer multiple of _pageSize greater than mapLen
    for (_mapLen = _pageSize; _mapLen < mapLen; _mapLen += _pageSize) {
        // pass
    }
}

OutputStream4::~OutputStream4() {
    close();
}

void OutputStream4::create(const char* const pathname) {
    close();
    _fd = ::open(pathname, O_RDWR|O_CREAT|O_TRUNC, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
    // TODO: we should try with and without the O_DIRECT flag
    //       (described in open(2)).

    if (_fd < 0) {
        throw FileOpenException(errno, "::open failed");
    }

    mapNext();
}

void OutputStream4::write(int_least32_t number) {
    if (_next + SIZE > _mapLen) {
        mapNext();
    }
    int32ToChars(&(_map[_next]), std::move(number));
    _next += SIZE;
}

void OutputStream4::close() {
    size_t fileSize(_fileSize);
    size_t next(_next);
    unmap();

    if (_fd != -1) {
        if (ftruncate(_fd, fileSize + next - _mapLen) != 0) {
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

void OutputStream4::mapNext() {
    char* oldMap(_map);
    unmap();

    // Increase the file size
    if (lseek(_fd, _mapLen, SEEK_CUR) < 0) {
        char message[30];
        snprintf(message, 29, "lseek to %ld (%ld + %ld - 1) failed", _fileSize+_mapLen-1, _fileSize, _mapLen);
        throw FileWriteException(errno, message);
    }
    if (::write(_fd, "", 1) < 1) {
        throw FileWriteException(errno, "::write failed");
    }

    void *map = mmap(oldMap, _mapLen, PROT_WRITE, MAP_SHARED, _fd, _fileSize);
    if (map == MAP_FAILED) {
        throw FileOpenException(errno, "mmap failed");
    }
    _fileSize += _mapLen;
    _map = reinterpret_cast<char*>(map);
    _next = 0;
}

/*
void OutputStream4::sync() {
    if (_map != nullptr) {
        if (msync(_map, _mapLen, MS_ASYNC) < 0) {
            throw FileCloseException(errno);
        }
    }
}
*/

void OutputStream4::unmap() {
    if (_map != nullptr) {
        if (munmap(_map, _mapLen) == 0) {
            _map = nullptr;
            _next = 0;
        }
        else {
            throw FileCloseException(errno, "unmap failed");
        }
    }
}

// vim: set shiftwidth=4 softtabstop=4 :
