OBJECTS=parser.tab.o lexer.yy.o parse_transform.o
SOURCES=*.cpp *.h
CFLAGS=-std=c++0x -g
LDFLAGS=

all: parse_transform

parse_transform: $(OBJECTS)
	g++ $(LDFLAGS) $(CFLAGS) -o parse_transform $^

clean:
	rm -f *.o lexer.yy.cpp parser.tab.cpp parser.tab.hpp parse_transform

parser.tab.cpp parser.tab.hpp: parsing/parser.ypp
	bison -d parsing/parser.ypp

lexer.yy.cpp: parsing/lexer.lex
	flex -+ -o lexer.yy.cpp parsing/lexer.lex

.cpp.o:
	g++ -c $(CFLAGS) -o $@ $<

.PHONY: all clean
