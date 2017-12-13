DEPS = $(wildcard *.hpp)
FLAGS = -Wpedantic -Wall -Wextra -Wconversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant

streams: main.cpp ${DEPS}
	g++ -std=c++11 -o $@ $^ -ggdb3 ${FLAGS}

nowarn: main.cpp ${DEPS}
	g++ -std=c++11 -o streams $^ -ggdb3

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

random: random.4 random.16 random.256 random.65536

test: streams test1.out test2.out test3.out test4.out

test1.out: streams data
	./streams 1
	diff -q data test1.out

test2.out: streams data
	./streams 2
	diff -q data test2.out

test3.out: streams data
	./streams buffered
	diff -q data test3.out

test4.out: streams data
	echo "/!\ test for memory mapping to do"

.PHONY: nowarn random test
