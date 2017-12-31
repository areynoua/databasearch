#include <cstdlib> // exit
#include <error.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
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

// TODO use madvise (linux; maybe not unix)
// TODO map all the file (e.g. when mapLen == 0)

const size_t InputStream4::_pageSize = sysconf(_SC_PAGESIZE); // The page size in bytes

InputStream4::InputStream4(size_t mapLen) {
    // TODO: check that the size accepted by the system

    // compute _mapLen as the lowest integer multiple of _pageSize greater than mapLen
    for (_mapLen = 0; _mapLen < mapLen; _mapLen += _pageSize) {
        // pass
    }
}

InputStream4::~InputStream4() {
    close();
}

void InputStream4::open(const char* const pathname) {
    close();
    _fd = ::open(pathname, O_RDONLY | O_LARGEFILE);
    if (_fd < 0) {
        throw FileOpenException(errno, "::open failed");
    }

    struct stat fileStat;
    if (fstat(_fd, &fileStat) < 0) {
        throw FileOpenException(errno, "fstat failed");
    }

    if (fileStat.st_size < 0) {
        exit(-2); // How is it even possible?
    }
    _fileSize = static_cast<size_t>(fileStat.st_size);

    map();
}

int_least32_t InputStream4::read_next() {
    int_least32_t value(0);

    if (_mapoff + _next + SIZE <= _fileSize) { // enough bytes in the file
        if (_next + SIZE > _mapLen) { // not enough mapped bytes
            map(_mapoff + _next);
        }
        value = charsToInt32(&(_map[_next]));
        _next += SIZE;
    }
    else { // not enough bytes in the file
        size_t remaining_bytes = _fileSize - (_mapoff + _next);
        if (remaining_bytes > 0) {
            error(0, 0, "Last %ld bytes ignored (%ld bytes expected)", remaining_bytes, SIZE);
        }
        // else remaining_bytes == 0 { EOF }
    }

    return value;
}

bool InputStream4::end_of_stream() {
    return _mapoff + _next >= _fileSize;
}

void InputStream4::close() {
    unmap();
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
    unmap();
    void *map = mmap(nullptr, _mapLen, PROT_READ, MAP_SHARED, _fd, offset);
    if (map == MAP_FAILED) {
        throw FileOpenException(errno, "map failed");
    }
    _map = reinterpret_cast<char*>(map);
    _mapoff = offset;
    _next = 0;
}

void InputStream4::unmap() {
    if (_map != nullptr) {
        if (munmap(_map, _mapLen) == 0) {
            _map = nullptr;
        }
        else {
            throw FileCloseException(errno);
        }
    }
}

// vim: set shiftwidth=4 softtabstop=4 :
