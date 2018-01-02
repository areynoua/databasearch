#ifndef DEF_INPUTSTREAM2
#define DEF_INPUTSTREAM2

#include "common.hpp"

class InputStream2 final : virtual public AbstractInputstream {
private:
    static const size_t SIZE = 32 / 8; // 32 bits
    FILE *file_pointer;
    int32_t read_value;
    ssize_t read_size;

public:
    InputStream2()
        : file_pointer(nullptr), read_value(0), read_size(0) {}
    InputStream2(const InputStream2&) = delete;
    InputStream2& operator=(const InputStream2&) = delete;
    virtual ~InputStream2();

    void open(const char* const) override;
    int32_t read_next() override;
    bool end_of_stream() override;
    void close() override;
};

#endif

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
