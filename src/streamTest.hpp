#ifndef __STREAMTEST_HPP__
#define __STREAMTEST_HPP__


inline int_least32_t rand32();

void generate(char ** argv);


void benchmarkInputStream(AbstractInputstream ** isVector, size_t vectorSize);

void benchmarkOutputStream(vector<AbstractOutputstream*> osVector, int min, int max);




#endif
