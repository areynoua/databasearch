#!/usr/bin/python3

from sys import argv

def templateArgs(i) :
    if i == 1 or i == 2 :
        return ('', )
    if i == 3 :
        return ('<{}>'.format(2**(2**p)) for p in range(5))
    if i == 4 :
        return ('<{}>'.format(i) for i in (1, 2, 4))

fout = open("run_all_stream_tests.hpp", "w")
print('// generated by',argv[0], file=fout)

print('#ifndef __RUN_ALL_STREAM_TESTS_HPP__', file=fout)
print('#define __RUN_ALL_STREAM_TESTS_HPP__', file=fout)
print('#include "../InputStream3.hpp"', file=fout)
print('#include "../OutputStream3.hpp"', file=fout)
print('#include "../InputStream4.hpp"', file=fout)
print('#include "../OutputStream4.hpp"\n', file=fout)
for i in range(1, 5) :
    l = tuple(templateArgs(i))
    if l != ('', ) :
        for si in l: 
            print('//template class OutputStream{0}{1};\ntemplate class InputStream{0}{1};'.format(i, si), file=fout)

print('bool run_all_stream_tests();', file=fout)
print('#endif', file=fout)
fout.close()

fout = open("run_all_stream_tests.cpp", "w")
print('// generated by',argv[0], file=fout)

print('#include <cstdlib>\n', file=fout)
print('#include "run_all_stream_tests.hpp"\n', file=fout)
print('#include "../InputStream1.hpp"', file=fout)
print('#include "../OutputStream1.hpp"', file=fout)
print('#include "../InputStream2.hpp"', file=fout)
print('#include "../OutputStream2.hpp"\n', file=fout)
print('#include "test_streams.hpp"\n', file=fout)

print("""
bool run_all_stream_tests() {
    int check(std::system(nullptr));
    if (!check) {
      std::cout << "! Unable to perform all the check" << std::endl;
    }
    bool passed = true;
    """, file=fout)
for i in range(1, 5) :
    for j in range(1, 5) :
        for si in templateArgs(i) :
            for sj in templateArgs(j) :
                print('    passed = metaTestStreams<OutputStream{0}{2}, InputStream{1}{3}>("test{0}{4}.{1}{5}", check) && passed;'.format(
                    i, j, si, sj,
                    '-{}'.format(si[1:-1]) if len(si) > 0 else '',
                    '-{}'.format(sj[1:-1]) if len(sj) > 0 else ''), file=fout)
print('    return passed;\n}', file=fout)

fout.close()
