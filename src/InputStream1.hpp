#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstdint>

#include "streams.hpp"

#ifndef DEF_INPUTSTREAM1
#define DEF_INPUTSTREAM1
using namespace std;

class InputStream1: virtual public AbstractInputstream{
private:
    signed char buffer[4];
    int fd;
    size_t end;

public:
    InputStream1():fd(0), end(1){}
    void open_file(const char*);
    void read_next();
    bool end_of_stream();
    void read_all();
    int_least32_t charsToInt32(signed char*);


};


void InputStream1::open_file(const char* filename) {
    fd = open(filename,O_RDONLY);
    read_next();
}


void InputStream1::read_next(){
    end = read(fd, buffer, sizeof(buffer));
    if (!end_of_stream()){
        cout << charsToInt32(buffer) << endl;
    }
}

bool InputStream1::end_of_stream(){
    return end == 0;
}

void InputStream1::read_all(){
    while (end != 0){
        read_next();
    }
}


int_least32_t InputStream1::charsToInt32 (signed char* chars) {
    return (static_cast<int_least32_t>(chars[0]) << 24)
         | (static_cast<int_least32_t>(chars[1]) << 16)
         | (static_cast<int_least32_t>(chars[2]) << 8)
         | (static_cast<int_least32_t>(chars[3]));
}

#endif
