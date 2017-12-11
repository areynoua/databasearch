#ifndef DEF_INPUTSTREAM2
#define DEF_INPUTSTREAM2

#include <iostream>
#include <cstdint>

#include "streams.hpp"

using namespace std;

class InputStream2: virtual public AbstractInputstream {
private:
    FILE *file_pointer;
    size_t end;
public:
    InputStream2():file_pointer(nullptr), end(1){}
    virtual void open_file(const char* const) override;
    virtual int_least32_t read_next() override;
    virtual bool end_of_stream() override;
    void read_all();


};


void InputStream2::open_file(const char* filename){
    file_pointer = fopen(filename, "r");

    fseek(file_pointer, 0, SEEK_SET);
    cout << read_next() << endl;
}

int_least32_t InputStream2::read_next(){
    signed char buffer[4];
    end = fread(buffer,4,1,file_pointer);
    if (!end_of_stream()){
        return charsToInt32(buffer);
    }

    return NULL;
}

bool InputStream2::end_of_stream(){
    return end == 0;
}

void InputStream2::read_all(){
    while (end != 0){
        int_least32_t value = read_next();
        if (value != NULL){
            cout << value <<endl;;
        }
    }
}














#endif
