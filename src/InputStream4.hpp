#ifndef DEF_INPUTSTREAM4
#define DEF_INPUTSTREAM4

#include <cstdint> // int_least32_t
#include <sys/stat.h>

#include "common.h"

class InputStream4 final : virtual public AbstractInputstream {
    static const size_t _pageSize = sysconf(_SC_PAGESIZE); // The page size in bytes
    static const size_t SIZE = 32 / 8; /* the number of bytes in 1 int32 */
    int _fd = -1;
    char* _mmap = nullptr; // The address of the mapping
    struct stat _fileStat; // The stats of the mapped file (TODO: only keep the used values)
    size_t _mapLen; // The size of the mapping in bytes
    size_t _next = 0; // The offset of the next bytes to read according to the address of the mapping
    size_t _mapoff = 0; // Offset of the mapping in the file. The actual position in the file is _mapoff + _next

public:
    InputStream4(size_t mapLen = 0);
    virtual ~InputStream4();
    void open(const char* const) override;
    int_least32_t read_next() override;
    bool end_of_stream() override;
    void close() override;

private:
    void map(size_t offset = 0);
}

#endif // DEF_INPUTSTREAM4
// vim: set shiftwidth=4 softtabstop=4 :
