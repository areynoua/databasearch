#ifndef DEF_OUTPUTSTREAM4
#define DEF_OUTPUTSTREAM4

#include <cstdint> // int_least32_t

#include "common.hpp"

class OutputStream4 final : virtual public AbstractOutputstream {
    static const size_t _pageSize;
    static const size_t SIZE = 32 / 8; /* the number of bytes in 1 int32 */
    int _fd = -1; // File descriptor
    char* _map = nullptr; // The address of the mapping
    size_t _fileSize = 0; // The size of the mapped file (in bytes)
    size_t _mapLen = 0; // The size of the mapping (in bytes)
    size_t _next = 0; // The offset of the next bytes to read according to the address of the mapping

public:
    OutputStream4(size_t mapLen = 0);
    OutputStream4(const OutputStream4&) = delete;
    OutputStream4& operator=(const OutputStream4&) = delete;
    virtual ~OutputStream4();

    void create(const char* const) override;
    void write(int_least32_t) override;
    void close() override;

private:
    void mapNext();
    // void sync();
    void unmap();
};

#endif // DEF_OUTPUTSTREAM4
// vim: set shiftwidth=4 softtabstop=4 :
