#include <error.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#include "OutputStream1.hpp"

using namespace std;

OutputStream1::~OutputStream1() {
    close();
}

void OutputStream1::create(const char* const filename) {
    fd = ::open(filename, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    // TODO: we should try with and without the O_DIRECT flag
    //       (described in open(2)).

    if (fd < 0) {
        throw FileOpenException(errno);
    }
}


void OutputStream1::write(int number) {
    char buffer[4] {0};
    int32ToChars(buffer, number);
    ssize_t written_size(::write(fd, buffer, sizeof(buffer)));

    if (written_size != 4) {
        if (written_size >= 0) {
            error(0, 0, "Write interrupted after %ld bytes", written_size);
        }
        else {
            throw FileWriteException(errno);
        }
    }
}

void OutputStream1::close() {
    if (fd != -1) {
        if (::close(fd) == 0) {
            fd = -1;
        }
        else {
            throw FileCloseException(errno);
        }
    }
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
