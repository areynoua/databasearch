#ifndef __TEST_STREAMS_HPP__
#define __TEST_STREAMS_HPP__

#include <cstdlib>

#include "../common.hpp"

#include "run_all_stream_tests.hpp"

bool testStreams(AbstractOutputstream & os, AbstractInputstream & is, const char* const desc, bool check = true);

template <typename OS, typename IS>
bool metaTestStreams(const char* const desc, bool check) {
    OS os;
    IS is;
    return testStreams(os, is, desc, check);
}

#endif
