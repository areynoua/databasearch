#include <cstring> // strcmp
#include <iostream>

void noop();

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Need argument" << std::endl;
  }

  if (strcmp(argv[1], "nothing") == 0) {
    noop();
    std::cout << "OK. Nothing done." << std::endl;
  }
}

void noop() {
  // no operation
}


