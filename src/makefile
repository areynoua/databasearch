DEPS = $(wildcard *.hpp)
FLAGS = "-ggdb3 -Wpedantic -Wall -Wextra -Wconversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant"

streams: main.cpp ${DEPS}
	g++ -std=c++11 -o $@ $^ -ggdb3 

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

test: streams
	./streams nothing

.PHONY: random test
