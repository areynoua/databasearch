#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

#include "streams.hpp"

#ifndef DEF_INPUTSTREAM1
#define DEF_INPUTSTREAM1
using namespace std;

class InputStream1: virtual public AbstractInputstream{
private:
    unsigned char buffer[4];
    int fd;
    size_t end;

public:
    InputStream1():fd(0), end(1){}
    void open_file(const char*);
    void read_next();
    bool end_of_stream();
    void read_all();


};


void InputStream1::open_file(const char* filename) {
    fd = open(filename,O_RDONLY);
    read_next();
}


void InputStream1::read_next(){
    end = read(fd, buffer, sizeof(buffer));
    if (!end_of_stream()){
        for (size_t i = 0; i < sizeof(buffer); ++i){
            cout << int(buffer[i]);
        }
        cout << endl;
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

#endif
