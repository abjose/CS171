OBJECTS=parser.tab.o lexer.yy.o raster.o shaded.o
SOURCES=*.cc *.hh
CFLAGS=-std=c++0x -g
LDFLAGS=

all: shaded

shaded: $(OBJECTS)
	g++ $(LDFLAGS) $(CFLAGS) -o shaded $^

clean:
	rm *.o lexer.yy.cpp parser.tab.cpp parser.tab.hpp shaded

parser.tab.cpp parser.tab.hpp: parsing/parser.ypp
	bison -d parsing/parser.ypp

lexer.yy.cpp: parsing/lexer.lex
	flex -+ -o lexer.yy.cpp parsing/lexer.lex

raster.o: raster/raster.h raster/raster.cpp
	g++ -c $(CFLGAS) -o $@ raster/raster.cpp

# necessary?
.cpp.o:
	g++ -c $(CFLAGS) -o $@ $<

.cc.o:
	g++ -c $(CFLAGS) -o $@ $<

.PHONY: all clean
