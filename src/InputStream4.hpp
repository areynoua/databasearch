#ifndef DEF_INPUTSTREAM4
#define DEF_INPUTSTREAM4

#include <cstdlib> // exit
#include <error.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h> // sysconf

#include "common.hpp"

// TODO use madvise (linux; maybe not unix)
// TODO map all the file (e.g. when mapLen == 0)
// TODO do not map unnecessary pages
// TODO: check that the size accepted by the system

template <int PAGEC> // number of page to map
class InputStream4 final : virtual public AbstractInputstream {
    static const size_t PAGESIZE; // The page size in bytes
    static const size_t SIZE; // The number of bytes in 1 int32
    static const size_t MAPLEN; // The size of the mapping (in bytes)
    int _fd = -1;
    char* _map = nullptr; // The address of the mapping
    size_t _fileSize = 0; // The size of the mapped file (in bytes)
    size_t _next = 0; // The offset of the next bytes to read according to the address of the mapping
    size_t _mapoff = 0; // Offset of the mapping in the file. The actual position in the file is _mapoff + _next

public:
    InputStream4() {};
    InputStream4(const InputStream4&) = delete;
    InputStream4& operator=(const InputStream4&) = delete;
    virtual ~InputStream4();
    void open(const char* const) override;
    int32_t read_next() override;
    bool end_of_stream() override;
    void close() override;

private:
    void map(size_t offset = 0);
    void unmap();
};
template <int PAGEC>
const size_t InputStream4<PAGEC>::PAGESIZE = sysconf(_SC_PAGESIZE);
template <int PAGEC>
const size_t InputStream4<PAGEC>::SIZE = 32 / 8; // the number of bytes in 1 int32
template <int PAGEC>
const size_t InputStream4<PAGEC>::MAPLEN = PAGEC*PAGESIZE; // The size of the mapping (in bytes)

template <int PAGEC>
InputStream4<PAGEC>::~InputStream4() {
    close();
}

template <int PAGEC>
void InputStream4<PAGEC>::open(const char* const pathname) {
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

template <int PAGEC>
int32_t InputStream4<PAGEC>::read_next() {
    int32_t value(0);

    if (_mapoff + _next + SIZE <= _fileSize) { // enough bytes in the file
        if (_next + SIZE > MAPLEN) { // not enough mapped bytes
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

template <int PAGEC>
bool InputStream4<PAGEC>::end_of_stream() {
    return _mapoff + _next >= _fileSize;
}

template <int PAGEC>
void InputStream4<PAGEC>::close() {
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

template <int PAGEC>
void InputStream4<PAGEC>::map(size_t offset) {
    unmap();
    void *map = mmap(nullptr, MAPLEN, PROT_READ, MAP_SHARED, _fd, offset);
    if (map == MAP_FAILED) {
        throw FileOpenException(errno, "map failed");
    }
    _map = reinterpret_cast<char*>(map);
    _mapoff = offset;
    _next = 0;
}

template <int PAGEC>
void InputStream4<PAGEC>::unmap() {
    if (_map != nullptr) {
        if (munmap(_map, MAPLEN) == 0) {
            _map = nullptr;
        }
        else {
            throw FileCloseException(errno);
        }
    }
}

#endif // DEF_INPUTSTREAM4
// vim: set shiftwidth=4 softtabstop=4 :
