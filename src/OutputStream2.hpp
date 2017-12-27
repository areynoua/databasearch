#ifndef DEF_OUTPUTSTREAM2
#define DEF_OUTPUTSTREAM2

#include "common.hpp"

using namespace std;

class OutputStream2 final : virtual public AbstractOutputstream {
private:
    FILE *file_pointer;
public:
    OutputStream2(): file_pointer(nullptr) {}
    OutputStream2(const OutputStream2&) = delete;
    OutputStream2& operator=(const OutputStream2&) = delete;
    virtual ~OutputStream2();

    void create(const char* const) override;
    void write(int_least32_t) override;
    void close() override;
};

#endif
// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
