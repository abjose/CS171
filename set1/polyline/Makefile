OBJECTS=parser.tab.o lexer.yy.o polyline.o
SOURCES=*.cpp *.h
CFLAGS=-std=c++0x -g
LDFLAGS=

all: polyline

polyline: $(OBJECTS)
	g++ $(LDFLAGS) $(CFLAGS) -o polyline $^

clean:
	rm *.o lexer.yy.cpp parser.tab.cpp parser.tab.hpp polyline

parser.tab.cpp parser.tab.hpp: parsing/parser.ypp
	bison -d parsing/parser.ypp

lexer.yy.cpp: parsing/lexer.lex
	flex -+ -o lexer.yy.cpp parsing/lexer.lex

.cpp.o:
	g++ -c $(CFLAGS) -o $@ $<

.PHONY: all clean
