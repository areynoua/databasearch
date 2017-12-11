#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
using namespace std;


#ifndef DEF_IMPL1
#define DEF_IMPL1


long getFileSize(int fd){
    struct stat stat_buffer;
    int rc = fstat(fd,&stat_buffer);
    return rc == 0 ? stat_buffer.st_size : -1;
}

void readFile() {
    int fd = open("random.65536",O_RDONLY);
    long filesize = getFileSize(fd);
    unsigned char buffer[filesize];
    read(fd, buffer, sizeof(buffer));
    for (int i = 0; i < sizeof(buffer); ++i){
        cout << int(buffer[i]);
    }
    cout << endl;
}


void writeFile(int number){

    
}


#endif
