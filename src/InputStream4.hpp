#ifndef DEF_INPUTSTREAM4
#define DEF_INPUTSTREAM4

#include <cstdint> // int_least32_t

#include "common.hpp"

class InputStream4 final : virtual public AbstractInputstream {
    static const size_t _pageSize;
    static const size_t SIZE = 32 / 8; /* the number of bytes in 1 int32 */
    int _fd = -1;
    char* _map = nullptr; // The address of the mapping
    size_t _fileSize = 0; // The size of the mapped file (in bytes)
    size_t _mapLen = 0; // The size of the mapping (in bytes)
    size_t _next = 0; // The offset of the next bytes to read according to the address of the mapping
    size_t _mapoff = 0; // Offset of the mapping in the file. The actual position in the file is _mapoff + _next

public:
    InputStream4(size_t mapLen = 0);
    InputStream4(const InputStream4&) = delete;
    InputStream4& operator=(const InputStream4&) = delete;
    virtual ~InputStream4();
    void open(const char* const) override;
    int_least32_t read_next() override;
    bool end_of_stream() override;
    void close() override;

private:
    void map(size_t offset = 0);
    void unmap();
};

#endif // DEF_INPUTSTREAM4
// vim: set shiftwidth=4 softtabstop=4 :
