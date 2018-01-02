Files
=====

generate_testdata.cc
--------------------

compile to generate_testdata which creates the testdata.* files.

tests.cc
--------

compile to tests

usage: tests [streams] [merge]

generate_run_all_stream_tests.py
--------------------------------

python3 script to create a function run_all_stream_tests() in the hpp file with
the same name.

run_all_merge_tests.*pp
-----------------------

used by tests.cc

test_streams.*pp
----------------

used by tests.cc though run_all_stream_tests.*pp


Some generated files
====================

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

run_all_stream_tests.*pp
------------------------

Contains the run_all_stream_tests function which run metaStreamTest on a lot of
different configurations of input and output streams.

