#ifndef __STREAMTEST_HPP__
#define __STREAMTEST_HPP__

inline int32_t rand32();

void generate(char ** argv);

void benchmarkInputStream(AbstractInputstream ** isVector, size_t vectorSize);

void benchmarkOutputStream(AbstractOutputstream ** osVector, size_t vectorSize, size_t filesSize);

#endif
