OBJECTS=matrix_tests.o
SOURCES=*.cpp *.h
CFLAGS=-std=c++0x -g
LDFLAGS=

all: tests

tests: $(OBJECTS)
	g++ $(LDFLAGS) $(CFLAGS) -o tests $^

clean:
	rm *.o tests

.cpp.o:
	g++ -c $(CFLAGS) -o $@ $<

.PHONY: all clean
