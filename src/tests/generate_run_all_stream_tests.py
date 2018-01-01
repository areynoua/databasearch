#!/usr/bin/python3

fout = open("run_all_stream_tests.hpp", "w")
print('void run_all_stream_tests();', file=fout)
fout.close()


fout = open("run_all_stream_tests.cpp", "w")

print('#include "run_all_stream_tests.hpp"\n', file=fout)
print('#include "../InputStream1.hpp"', file=fout)
print('#include "../OutputStream1.hpp"', file=fout)
print('#include "../InputStream2.hpp"', file=fout)
print('#include "../OutputStream2.hpp"', file=fout)
print('#include "../InputStream3.hpp"', file=fout)
print('#include "../OutputStream3.hpp"', file=fout)
print('#include "../InputStream4.hpp"', file=fout)
print('#include "../OutputStream4.hpp"\n', file=fout)
print('#include "test_streams.hpp"\n', file=fout)

def templateArgs(i) :
    if i == 1 or i == 2 :
        return ('', )
    if i == 3 :
        return ('<{}>'.format(2**(2**p)) for p in range(5))
    if i == 4 :
        return ('<{}>'.format(i) for i in (1, 2, 4))

print('void run_all_stream_tests() {', file=fout)
for i in range(1, 5) :
    for j in range(1, 5) :
        for si in templateArgs(i) :
            for sj in templateArgs(j) :
                print('    metaTestStreams<OutputStream{0}{2}, InputStream{1}{3}>("test{0}{4}.{1}{5}");'.format(
                    i, j, si, sj,
                    '-{}'.format(si[1:-1]) if len(si) > 0 else '',
                    '-{}'.format(sj[1:-1]) if len(sj) > 0 else ''), file=fout)
print('}', file=fout)

fout.close()
