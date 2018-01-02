#ifndef DEF_INPUTSTREAM1
#define DEF_INPUTSTREAM1

#include "common.hpp"

/**
 * Stream on 32 bits without buffer
 * has a "buffer" of 4 bytes
 */
class InputStream1 final : virtual public AbstractInputstream {
    static const size_t SIZE = 32 / 8; // 32 bits
    int fd;
    char buffer[SIZE];
    ssize_t read_size;

public:
    InputStream1(): fd(-1), read_size(0) {}
    virtual ~InputStream1();

    void open(const char* const) override;
    int32_t read_next() override;
    inline bool end_of_stream() override;
    void close() override;
};

#endif

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
