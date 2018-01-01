#include "../common.hpp"

void testStreams(AbstractOutputstream & os, AbstractInputstream & is, const char* const desc);

template <typename OS, typename IS>
void metaTestStreams(const char* const desc);
