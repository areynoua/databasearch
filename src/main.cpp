#include <cstring> // strcmp
#include <iostream>
#include "memoryMapping.hpp"
#include "InputStream1.hpp"
#include "OutputStream1.hpp"
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
        os.write_file(111);
        //      os.write_file(-2);
        //      os.write_file(3);
        //      os.write_file(34652);
        os.close_file();
        InputStream1 is = InputStream1();
        is.open_file(argv[2]);
        is.read_all();
    }
    else if (strcmp(argv[1], "buffered") == 0) {
        BufferedInputstream<2> bis = BufferedInputstream<2>();
        bis.open_file("random.16");
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

// vim: set shiftwidth=4:
