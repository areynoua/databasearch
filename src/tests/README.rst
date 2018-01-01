Files
=====

test_streams.cpp
----------------

compile to test_streams

generate_testdata.cpp
---------------------

compile to generate_testdata which creates the testdata.* files.

generate_run_all_stream_tests.py
--------------------------------

python3 script to create a function runAllStreamsTests in the hpp file with the
same name.

Generated files
===============

test_streams
------------

tests the differents streams

generate_testdata
-----------------

To execute without any argument.
Generate testdata.* files.

testdata.*
----------

Contain random int32 s.
testdata.bin contains the data in a binary format.
testdata.txt contains an int32 by line in a human readable format.
testdata.hpp define the const int32_t C TESTDATA array.

run_all_stream_tests.hpp
------------------------

Contains a function which run all the metaStreamTest
