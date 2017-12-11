#include <cstring> // strcmp
#include <iostream>
#include "InputStream1.hpp"
#include "OutputStream1.hpp"
#include "OutputStream3.hpp"

void noop();

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Need argument" << std::endl;
  }

  if (strcmp(argv[1], "nothing") == 0) {
    noop();
    std::cout << "OK. Nothing done." << std::endl;
  }

  if (strcmp(argv[1], "1") == 0){
      OutputStream1 os = OutputStream1();
      os.create(argv[2]);
      os.write_file(1);
      os.write_file(2);
      os.write_file(3);
      os.write_file(4);
      os.close_file();
      InputStream1 is = InputStream1();
      is.open_file(argv[2]);
      is.read_all();
  }

  if (strcmp(argv[1], "buffered") == 0) {
    BufferedInputstream<8> bis = BufferedInputstream<8>();
    bis.open_file("random.16");
    for (int i = 0; i < 4; ++i) {
      std::cout << bis.read_next() << std::endl;
    }
    std::cout << bis.end_of_stream() << std::endl;
  }
}

void noop() {
  // no operation
}
