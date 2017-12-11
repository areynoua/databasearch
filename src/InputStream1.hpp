#ifndef DEF_INPUTSTREAM1
#define DEF_INPUTSTREAM1

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstdint>

#include "streams.hpp"

using namespace std;

class InputStream1: virtual public AbstractInputstream {
private:
    signed char buffer[4];
    int fd;
    size_t end;

public:
    InputStream1():fd(0), end(1){}
    virtual void open_file(const char* const) override;
    virtual int_least32_t read_next() override;
    virtual bool end_of_stream() override;

    void read_all();
};


void InputStream1::open_file(const char* filename) {
    fd = open(filename,O_RDONLY);
    read_next();
}


int_least32_t InputStream1::read_next(){
    end = read(fd, buffer, sizeof(buffer));
    if (!end_of_stream()){
        return charsToInt32(buffer);
    }
    return 0; // TODO
}

bool InputStream1::end_of_stream(){
    return end == 0;
}

void InputStream1::read_all(){
    while (end != 0){
        read_next();
    }
}

#endif

// vim: set shiftwidth=4:
