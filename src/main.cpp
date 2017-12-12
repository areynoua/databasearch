#include <cstring> // strcmp
#include <iostream>
#include "memoryMapping.hpp"
#include "InputStream1.hpp"
#include "OutputStream1.hpp"
#include "InputStream2.hpp"
#include "OutputStream2.hpp"
#include "InputStream3.hpp"

void noop();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Need argument" << std::endl;
    }
    if (strcmp(argv[1], "nothing") == 0) {
        noop();
        std::cout << "OK. Nothing done." << std::endl;
    }
    else if (strcmp(argv[1], "1") == 0) {
        OutputStream1 os = OutputStream1();
        os.create(argv[2]);
        os.write_file(111); // FIXME: write_file behave well only when no implicit conversion occurs on the method paramter
        os.write_file(-2);
        os.write_file(3);
        os.write_file(-30884);
        os.close();

        std::cout << "expected: 111 -2 3 -30884" << std::endl;
        InputStream1 is = InputStream1();
        is.open(argv[2]);
        print_all(is);
    }
    else if (strcmp(argv[1], "2") == 0) {
        OutputStream1 os = OutputStream1();
        os.create(argv[2]);
        os.write_file(111);
        os.write_file(-45);
        InputStream2 is = InputStream2();
        is.open(argv[2]);
        is.read_all();
    }
    else if (strcmp(argv[1], "buffered") == 0) {
        BufferedInputstream<2> bis = BufferedInputstream<2>();
        bis.open("random.16");
        for (int i = 0; i < 4; ++i) {
            std::cout << bis.read_next() << std::endl;
        }
        std::cout << bis.end_of_stream() << std::endl;
    }
    else {
        // TESTING MEMORY_MAPPING, IMPLEMENTATION 4
        char *data;
        char* filename = argv[1];
        //MemoryMapping mmap = MemoryMapping(filename, 16);
        MemoryMapping mmap = MemoryMapping(filename);
        //data = mmap.writeFile();
        data = mmap.read_file();
    }

    return 0;
}

void noop() {
    // no operation
}

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
