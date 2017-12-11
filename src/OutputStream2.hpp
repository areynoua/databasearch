#include <unistd.h>
#include <iostream>

#include "streams.hpp"

#ifndef DEF_OUTPUTSTREAM2
#define DEF_OUTPUTSTREAM2
using namespace std;

class OutputStream2: virtual public AbstractOutputstream{
private:
    FILE *file_pointer;
public:
    OutputStream2(): file_pointer(nullptr){}
    void create(const char*);
    void write_file(int);
    void close_file();
};


void OutputStream2::create(const char* filename){
    file_pointer = fopen(filename,"w");
}


void OutputStream2::write_file(int number){
    signed char buffer[4];
    buffer[0] = static_cast<signed char>((number >> 24) & 0xFF);
    buffer[1] = static_cast<signed char>((number >> 16) & 0xFF);
    buffer[2] = static_cast<signed char>((number >> 8) & 0xFF);
    buffer[3] = static_cast<signed char>(number & 0xFF);

    fwrite(buffer, sizeof(buffer),1,file_pointer);
}

void OutputStream2::close_file(){
    fclose(file_pointer);
}


#endif
