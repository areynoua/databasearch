#include <cstring> // strcmp
#include <iostream>
#include "impl1.hpp"



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
      InputStream1 is = InputStream1();
      is.open_file(argv[2]);
      is.read_all();
  }
}

void noop() {
  // no operation
}
