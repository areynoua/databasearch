#ifndef __STREAMS_HPP__
#define __STREAMS_HPP__

#include <cstdint>

int_least32_t charsToInt32 (signed char* chars) {
    return (static_cast<int_least32_t>(chars[0]) << 24)
         | (static_cast<int_least32_t>(chars[1]) << 16)
         | (static_cast<int_least32_t>(chars[2]) << 8)
         | (static_cast<int_least32_t>(chars[3]));
}

class AbstractInputstream {
public:
  virtual ~AbstractInputstream() = default;

  virtual void open_file(const char* const infilename) = 0;
  virtual int_least32_t read_next() = 0;
  virtual bool end_of_stream() = 0;
};

class AbstractOutputstream {
public:
  virtual ~AbstractOutputstream() = default;

  virtual void create(const char* const) = 0;
  virtual void write_file (int_least32_t elem) = 0;
  virtual void close_file() = 0;
};

#endif //__STREAMS_HPP__
