#include <iostream>

#include "InputStream3.hpp"
#include "OutputStream3.hpp"
#include "common.hpp"
#include "merge.hpp"

typedef InputStream3<512> InputStream;
typedef OutputStream3<1024> OutputStream;

int main(int argc, char ** argv) {
  if (argc != 4) {
    std::cout << "usage: " << argv[0] << " filename M d\n"
      << "filename: 32-bits integer file (binary)\n"
      << "M: the number of binary integer to read at once from the file\n"
      << "d: the number of streams to merge at once" << std::endl;
    return 1;
  }

  // Input file
  InputStream input;
  input.open(argv[1]);

  // M
  int memory = atoi(argv[2]);
  if (memory < 1) {
    std::cout << argv[2] << " is not a valid value for M" << std::endl;
    return 1;
  }

  int streams = atoi(argv[3]);
  if (streams < 2) {
    std::cout << argv[2] << " is not a valid value for d" << std::endl;
    return 1;
  }

  char * out_filename = dway_merge_sort<OutputStream, InputStream>(input, memory, streams);

  std::cout << "Result stored in " << out_filename << std::endl;

  return 0;
}
