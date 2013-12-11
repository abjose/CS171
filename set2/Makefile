OBJECTS=parser.tab.o lexer.yy.o wireframe.o
SOURCES=*.cc *.hh
CFLAGS=-std=c++0x -g
LDFLAGS=

all: wireframe

wireframe: $(OBJECTS)
	g++ $(LDFLAGS) $(CFLAGS) -o wireframe $^

clean:
	rm *.o lexer.yy.cpp parser.tab.cpp parser.tab.hpp wireframe

parser.tab.cpp parser.tab.hpp: parsing/parser.ypp
	bison -d parsing/parser.ypp

lexer.yy.cpp: parsing/lexer.lex
	flex -+ -o lexer.yy.cpp parsing/lexer.lex

# necessary?
.cpp.o:
	g++ -c $(CFLAGS) -o $@ $<

.cc.o:
	g++ -c $(CFLAGS) -o $@ $<

.PHONY: all clean
