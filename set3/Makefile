OBJECTS=parser.tab.o lexer.yy.o raster.o lighting.o shaded.o
SOURCES=*.cc *.hh
CFLAGS=-std=c++0x -Wall -g #-pedantic -Wall -Wextra
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

raster.o: raster/raster.h raster/raster.cpp
	g++ -c $(CFLAGS) -o $@ raster/raster.cpp

#lexer.yy.o: lexer.yy.cpp
#	g++ -c -std=c++0x -g -o $@ lexer.yy.cpp

# necessary?
.cpp.o:
	g++ -c $(CFLAGS) -o $@ $<

.cc.o:
	g++ -c $(CFLAGS) -o $@ $<

.PHONY: all clean
