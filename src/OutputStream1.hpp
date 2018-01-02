#ifndef DEF_OUTPUTSTREAM1
#define DEF_OUTPUTSTREAM1

#include "common.hpp"

using namespace std;

class OutputStream1 final : virtual public AbstractOutputstream {
    static const size_t SIZE;
    int fd;
public:
    OutputStream1(): fd(-1) {}
    virtual ~OutputStream1();

    void create(const char* const) override;
    void write(int32_t elem) override;
    void close() override;
};

#endif
// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
