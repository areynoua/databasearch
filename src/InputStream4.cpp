#include <sys/mman.h>
#include <unistd.h> // sysconf

#include "InputStream4.hpp"

/*
 * void * mmap (void *addr,
 *              size_t len,
 *              int prot,
 *              int flags,
 *              int fd,
 *              off_t offset);
 */

InputStream4::InputStream4(size_t mapLen)
{
    // compute _mapLen as the lowest integer multiple of _pageSize greater than mapLen
    for (_mapLen = 0; _mapLen < mapLen; _mapLen += _pageSize) {
        // pass
    }
}

InputStream4::~InputStream4() {
    close();
}

void InputStream4::open(const char* const pathname) {
    _fd = ::open(pathname, O_RDONLY | O_LARGEFILE);
    if (_fd < 0) {
        throw FileOpenException(errno);
    }

    if (fstat(_fd, &_fileStat) < 0) {
        throw FileOpenException(errno);
    }

    map();
}

void InputStream4::read_next() {
    int_least32_t value(0);

    if (_mapoff + _next + SIZE <= _fileStat.st_size) { // enough bytes in the file
        if (_next + SIZE > _mapLen) { // not enough mapped bytes
            map(_mmapoff + _next);
        }
        value = charsToInt32(&(_mmap[_next]));
        _next += SIZE;
    }
    else { // not enough bytes in the file
        size_t remaining_bytes = _files.st_size - (_mmapoff + _next);
        if (remaining_bytes > 0) {
            error(0, 0, "Last %ld bytes ignored (%ld bytes expected)", remaining_bytes, SIZE);
        }
        // else remaining_bytes == 0 { EOF }
    }
    
    return value;
}

bool InputStream4::end_of_stream() {
    return _mmapoff + _next >= _fileStat.st_size;
}

InputStream4::close() {
    if (_mapp != nullptr) {
        if (munmap(_mmap, _mapLen) == 0) {
            _mmap = nullptr;
        }
        else {
            throw FileCloseException(errno);
        }
    }
    if (_fd != -1) {
        if (::close(_fd) == 0) {
            _fd = -1;
        }
        else {
            throw FileCloseException(errno);
        }
    }
}

void InputStream4::map(size_t offset) {
    void *mmap = mmap(offset, _mapLen, PROT_READ, MAP_SHARED, _fd, 0);
    if (mmap == MAP_FAILED) {
        throw FileOpenException(errno);
    }
    _mmap = reinterpret_cast<char*>(mmap);
    _mapoff = offset;
    _next = 0;
}

// vim: set shiftwidth=4 softtabstop=4 :
