#ifndef __STREAMS_HPP__
#define __STREAMS_HPP__

#include <string>

using string = std::string;

class AbstractInputstream {
public:
  void open_file(string infilename);
  int_least32_t read_next();
  bool end_of_stream();
};

class AbstractIutputstream {
public:
  void create(string outfilename);
  void write (int_least32_t elem);
  void close();
};

#endif //__STREAMS_HPP__
