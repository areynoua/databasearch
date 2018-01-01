#include <cstring>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <utility>

// ! binary will be executed from parent directory ! //

#include "../OutputStream2.hpp"

#include "../common.hpp"

inline int_least32_t rand32() {
  char chars[4];
  chars[0] = static_cast<char>(std::rand());
  chars[1] = static_cast<char>(std::rand());
  chars[2] = static_cast<char>(std::rand());
  chars[3] = static_cast<char>(std::rand());
  return charsToInt32(chars);
}

int main() {
  size_t len = 1030;

  OutputStream2 os;
  std::ofstream otxts;
  std::ofstream ohpps;
  std::srand(1u);

  os.create("tests/testdata.bin");
  otxts.open("tests/testdata.txt", ios_base::trunc | ios_base::out);
  ohpps.open("tests/testdata.hpp", ios_base::trunc | ios_base::out);
  ohpps << "#include <cstdint>\n#include <cstddef>\n\n"
    << "const size_t TESTDATA_SIZE = " << len << ";\n"
    << "const char TESTDATA_FILENAME[] = \"tests/testdata.bin\";\n"
    << "const int32_t TESTDATA[TESTDATA_SIZE] = {";

  for (size_t i = 0; i < len; ++i) {
    int32_t value = rand32();
    os.write(value);
    otxts << value << "\n";
    ohpps << value << ", ";
  }

  otxts << std::flush;
  otxts.close();
  ohpps << "};" << std::endl;
  ohpps.close();
  os.close();

  return 0;
}
