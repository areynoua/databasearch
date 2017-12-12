#include <unistd.h>
#include <iostream>
#include <fcntl.h>

#include "common.hpp"

#ifndef DEF_OUTPUTSTREAM1
#define DEF_OUTPUTSTREAM1
using namespace std;

class OutputStream1: virtual public AbstractOutputstream{
private:
    int fd;
public:
    OutputStream1(): fd(0){}
    void create(const char*);
    void write_file(int);
    void close();
};


void OutputStream1::create(const char* filename){
    fd = ::open(filename, O_RDWR|O_CREAT, 0666);
}


void OutputStream1::write_file(int number){
    signed char buffer[4];
    buffer[0] = static_cast<signed char>((number >> 24) & 0xFF);
    buffer[1] = static_cast<signed char>((number >> 16) & 0xFF);
    buffer[2] = static_cast<signed char>((number >> 8) & 0xFF);
    buffer[3] = static_cast<signed char>(number & 0xFF);

    write(fd, buffer, sizeof(buffer));
    read(fd, buffer, sizeof(buffer));
}

void OutputStream1::close(){
    ::close(fd);
}


#endif
// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
