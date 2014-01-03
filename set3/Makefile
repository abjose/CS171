OBJECTS=parser.tab.o lexer.yy.o lighting.o raster.o shaded.o
SOURCES=*.cc *.hh
CFLAGS=-std=c++0x -g #-pedantic #-Wall #-Wextra
LDFLAGS=

all: shaded

shaded: $(OBJECTS)
	g++ $(LDFLAGS) $(CFLAGS) -o shaded $^

clean:
	rm -f *.o lexer.yy.cpp parser.tab.cpp parser.tab.hpp shaded

parser.tab.cpp parser.tab.hpp: parsing/parser.ypp
	bison -d parsing/parser.ypp

lexer.yy.cpp: parsing/lexer.lex
	flex -+ -o lexer.yy.cpp parsing/lexer.lex

lighting.o: lighting/lighting.hh lighting/lighting.cc
	g++ -c $(CFLAGS) -o $@ lighting/lighting.cc

raster.o: raster/raster.h raster/raster.cpp
	g++ -c $(CFLAGS) -o $@ raster/raster.cpp

# necessary?
.cpp.o:
	g++ -c $(CFLAGS) -o $@ $<

.cc.o:
	g++ -c $(CFLAGS) -o $@ $<

.PHONY: all clean
