CXX = g++

SRCS = $(wildcard *.cpp)
DEPS = $(wildcard *.hpp)
OBJS = $(subst .cpp,.o,${SRCS})
CXXFLAGS = -std=c++11 -ggdb3 -Wpedantic -Wall -Wextra -Wconversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant


# Compile
streams: ${OBJS}
	$(CXX) ${CXXFLAGS} -o $@ ${OBJS}

# dependences
%.d: %.cpp
	@set -e; rm -f $@; \
		$(CXX) -MM ${CXXFLAGS} $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

include $(SRCS:.cpp=.d)


# Run tests
test: streams test1.out test2.out test3.out test4.out test-merge

# data file contains the 8-bits integers:
# 97 0 0 0 65 0 0 0 1 1 1 1 1 0 1 0 253 255 255 255
# On a little-endian systems (like intel x86), this correspond to the 5 following signed integers:
# 97 65 16843009 65537 -3
data:
	env echo -ne '\x61\x00\x00\x00\x41\x00\x00\x00\x01\x01\x01\x01\x01\x00\x01\x00\xfd\xff\xff\xff' > ./data

# other data files may be generated with
# $ ./streams generate size size size
# each size is the number of int32 in a file
# $ ./streams generate 4 4 12
# will generate data.1 with 4 int32, data.2 with 4 int32 and data.3 with 12 int32
# as well as 3 data.x.txt with the corresponding number as decimal text
# 2 calls to `./streams generate` with the same arguments will produce the same files

# sorted.1 contains -2 4 7 7
# sorted.2 contains -3 4 9 10
# sorted.3 contains  3 3 4 5
sorted.1:
	env echo -ne '\xfe\xff\xff\xff\x04\x00\x00\x00\x07\x00\x00\x00\x07\x00\x00\x00' > ./sorted.1

sorted.2:
	env echo -ne '\xfd\xff\xff\xff\x04\x00\x00\x00\x09\x00\x00\x00\x0A\x00\x00\x00' > ./sorted.2

sorted.3:
	env echo -ne '\x03\x00\x00\x00\x03\x00\x00\x00\x04\x00\x00\x00\x05\x00\x00\x00' > ./sorted.3

sorted.123:
	env echo -ne '\xfd\xff\xff\xff\xfe\xff\xff\xff\x03\x00\x00\x00\x03\x00\x00\x00' > ./sorted.123
	env echo -ne '\x04\x00\x00\x00\x04\x00\x00\x00\x04\x00\x00\x00\x05\x00\x00\x00' >>./sorted.123
	env echo -ne '\x07\x00\x00\x00\x07\x00\x00\x00\x09\x00\x00\x00\x0a\x00\x00\x00' >>./sorted.123

test1.out: streams data
	./streams 1
	@diff -q data test1.out

test2.out: streams data
	./streams 2
	@diff -q data test2.out

test3.out: streams data
	./streams 3
	@diff -q data test3.out

test4.out: streams data
	@echo "/!\ test for memory mapping to do"

test-merge: streams sorted.1 sorted.2 sorted.3 sorted.123
	./streams merge 1
	@diff -q sorted.1 merge.out
	./streams merge 3
	@diff -q sorted.123 merge.out
	rm merge.out

# Generate random files
random: random.4 random.16 random.256 random.65536

random.4:
	head --bytes=4 /dev/urandom > random.4

random.16:
	head --bytes=16 /dev/urandom > random.16

random.256:
	head --bytes=256 /dev/urandom > random.256

random.65536:
	head --bytes=65536 /dev/urandom > random.65536

random.4294967296:
	head --bytes=4294967296 /dev/urandom > random.4294967296


# Clean
clean:
	rm -f ${OBJS}

cleanall: clean
	rm -f *~ *.default

.PHONY: random test clean cleanall test-merge


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

