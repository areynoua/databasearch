CXX = g++
SHELL = bash
TIME = /usr/bin/time
# find it with
# whereis -b time | cut -f 2 -d ' '
ME = $(shell whoami)_$(shell uname -n)

SRCS = $(wildcard *.cpp)
DEPS = $(wildcard *.hpp)
OBJS = $(subst .cpp,.o,${SRCS})
CXXFLAGS = -std=c++11 -O1 -Wpedantic -Wall -Wextra -Wconversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant

all:
	make test-streams
	make test-merge
	make test-merge-sort
	@echo "TESTS OK, run make clean to erase temporay files"
	@echo
	make benchmark-streams
	make benchmark-mergesort

#
# Binaries
#
streams: streams.cc ${OBJS}
	@echo compile $@
	$(CXX) ${CXXFLAGS} $^ -o $@

mergesort: mergesort.cc ${OBJS}
	@echo compile $@
	$(CXX) ${CXXFLAGS} $^ -o $@

tests/generate_testdata: tests/generate_testdata.cc ${OBJS}
	@echo compile $@
	$(CXX) ${CXXFLAGS} $^ -o $@

tests/tests: tests/tests.cc ${OBJS} tests/test_streams.cpp tests/run_all_stream_tests.cpp tests/run_all_stream_tests.hpp tests/run_all_merge_tests.cpp tests/run_all_merge_tests.hpp tests/testdata.hpp
	@echo compile $@
	$(CXX) ${CXXFLAGS} $^ -o $@

#
# Dependences
#
tests/testdata.hpp tests/testdata.bin : tests/generate_testdata
	tests/generate_testdata

tests/run_all_stream_tests.hpp tests/run_all_stream_tests.cpp: tests/generate_run_all_stream_tests.py
	cd tests && python3 generate_run_all_stream_tests.py && cd ..

#
# Make rules
#
%.d: %.cpp
	@set -e; rm -f $@; \
		$(CXX) -MM ${CXXFLAGS} $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

include $(SRCS:.cpp=.d)

#
# Perform tests
#
test-streams: tests/tests tests/testdata.bin tests/random.65536
	tests/tests streams
	@echo
	@echo $@ done
	@echo

test-merge: tests/tests tests/sorted.1 tests/sorted.2 tests/sorted.3 tests/merged.1 tests/merged.2 tests/merged.3
	tests/tests merge
	@echo
	@echo $@ done
	@echo

test-merge-sort: mergesort tests/data.123 tests/merged.3
	./mergesort tests/data.123 2 5 | cut -d ' ' -f 4 > /tmp/outfilename
	diff tests/merged.3 $(shell cat /tmp/outfilename)
	@echo
	@echo $@ done
	@echo

tests/sorted.1:
	@echo generate $@ with -2 4 7 7
	@env echo -ne '\xfe\xff\xff\xff\x04\x00\x00\x00\x07\x00\x00\x00\x07\x00\x00\x00' > tests/sorted.1

tests/sorted.2:
	@echo generate $@ with -3 4 9 10
	@env echo -ne '\xfd\xff\xff\xff\x04\x00\x00\x00\x09\x00\x00\x00\x0A\x00\x00\x00' > tests/sorted.2

tests/sorted.3:
	@echo generate $@ with 3 3 4 5
	@env echo -ne '\x03\x00\x00\x00\x03\x00\x00\x00\x04\x00\x00\x00\x05\x00\x00\x00' > tests/sorted.3

tests/data.123:
	@echo generate $@ with 7 -2 4 7 10 4 -3 9 5 4 3 3
	@env echo -ne '\x07\x00\x00\x00\xfe\xff\xff\xff\x04\x00\x00\x00\x07\x00\x00\x00' > tests/data.123
	@env echo -ne '\x0A\x00\x00\x00\x04\x00\x00\x00\xfd\xff\xff\xff\x09\x00\x00\x00' >>tests/data.123
	@env echo -ne '\x05\x00\x00\x00\x04\x00\x00\x00\x03\x00\x00\x00\x03\x00\x00\x00' >>tests/data.123

tests/merged.1:
	@echo generate $@ with -2 4 7 7
	@env echo -ne '\xfe\xff\xff\xff\x04\x00\x00\x00\x07\x00\x00\x00\x07\x00\x00\x00' > tests/merged.1

tests/merged.2:
	@echo generate $@ with -3 -2 4 4 7 7 9 10
	@env echo -ne '\xfd\xff\xff\xff\xfe\xff\xff\xff\x04\x00\x00\x00\x04\x00\x00\x00' > tests/merged.2
	@env echo -ne '\x07\x00\x00\x00\x07\x00\x00\x00\x09\x00\x00\x00\x0a\x00\x00\x00' >>tests/merged.2

tests/merged.3:
	@echo generate $@ with -3 -2 3 3 4 4 4 5 7 7 9 10
	@env echo -ne '\xfd\xff\xff\xff\xfe\xff\xff\xff\x03\x00\x00\x00\x03\x00\x00\x00' > tests/merged.3
	@env echo -ne '\x04\x00\x00\x00\x04\x00\x00\x00\x04\x00\x00\x00\x05\x00\x00\x00' >>tests/merged.3
	@env echo -ne '\x07\x00\x00\x00\x07\x00\x00\x00\x09\x00\x00\x00\x0a\x00\x00\x00' >>tests/merged.3

tests/random.%:
	@echo generate $@ with $(subst tests/random.,,$@) bytes of random data
	@head --bytes=$(subst tests/random.,,$@) /dev/urandom > $@

#
# Perform benchmarks
#
benchmark-streams: benchmark/$(ME)/streams.csv

benchmark-mergesort: benchmark/$(ME)/mergesort.csv

benchmark/$(ME)/config.txt: benchmark-config.sh
	mkdir -p $@
	@echo
	@echo "Will now try to show some interesting characteristics of the hardware to $@"
	@echo "This require super user permissions"
	@echo "If it fails: try \`sudo ./benchmark-config.sh $@\` or manually write the \"$@\" file"
	@echo
	sudo ./benchmark-config.sh $@

benchmark/$(ME)/streams.csv: benchmark/${ME}/config.txt streams benchmark-streams.sh
	sudo ./benchmark-streams.sh $@ || ./benchmark-streams.sh $@

benchmark/$(ME)/mergesort.csv: benchmark/${ME}/config.txt mergesort benchmark-mergesort.sh "output data/od.0"
	sudo ./benchmark-mergesort.sh $@ || ./benchmark-mergesort.sh $@

# Clean
clean:
	rm -f ${OBJS}

cleanall: clean
	rm -f *~ *.default
	rm -f */*~ */*.default
	rm -f *.d.*
	rm -f tests/test*.data.out tests/test*.random.out tests/random.*
	rm -f tests/sorted.* tests/merged.* tests/merge.*.out tests/data.*

mrproper: clean cleanall
	rm -f *.d
	#rm `git ls-files -i --exclude-from=.gitignore`

.PHONY: all clean cleanall mrproper test-merge test-streams


## compile streams
#streams: main.cpp ${DEPS}
#	g++ -std=c++11 -o $@ $^ ${FLAGS}
#
## compile streams without warnings
#nowarn: main.cpp ${DEPS}
#	g++ -std=c++11 -o streams $^ -ggdb3
#	touch nowarn
#
#%.o: %.c ${COMMON_DEPS}
#
#streams: TODO les fichiers .o
#	$(CXX) -std=c++11 -o $@ $^ ${FLAGS}
#
#depend: .depend
#
#.depend: ${SRCS}
#	rm -f ./.depend
#	$(CXX) ${FLAGS} -MM $^ >> ./.depend
#
#include .depend

## Run tests
#test: streams test1.out test2.out test3.out test4.out test-merge
#
## data file contains the 8-bits integers:
## 97 0 0 0 65 0 0 0 1 1 1 1 1 0 1 0 253 255 255 255
## On a little-endian systems (like intel x86), this correspond to the 5 following signed integers:
## 97 65 16843009 65537 -3
#data:
#	env echo -ne '\x61\x00\x00\x00\x41\x00\x00\x00\x01\x01\x01\x01\x01\x00\x01\x00\xfd\xff\xff\xff' > ./data
#
## other data files may be generated with
## $ ./streams generate size size size
## each size is the number of int32 in a file
## $ ./streams generate 4 4 12
## will generate data.1 with 4 int32, data.2 with 4 int32 and data.3 with 12 int32
## as well as 3 data.x.txt with the corresponding number as decimal text
## 2 calls to `./streams generate` with the same arguments will produce the same files
#
#test1.out: streams data
#	./streams 1
#	@diff -q data test1.out
#	@diff -q random.65536 random.cpy
#
#test2.out: streams data
#	./streams 2
#	@diff -q data test2.out
#	@diff -q random.65536 random.cpy
#
#test3.out: streams data
#	./streams 3
#	@diff -q data test3.out
#	@diff -q random.65536 random.cpy
#
#test4.out: streams data
#	./streams 4
#	@diff -q data test4.out
#	@diff -q random.65536 random.cpy
#
#test23.out: streams data
#	./streams 23
#	@diff -q data test23.out
#	@diff -q random.65536 random.cpy
#
#test32.out: streams data
#	./streams 32
#	@diff -q data test32.out
#	@diff -q random.65536 random.cpy
#
#test24.out: streams data
#	./streams 24
#	@diff -q data test24.out
#	@diff -q random.65536 random.cpy
#
#test42.out: streams data
#	./streams 42
#	@diff -q data test42.out
#	@diff -q random.65536 random.cpy

