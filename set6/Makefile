# Makefile for cs171 HW 6

CXX		= g++
# add -g for debugging info
CXXFLAGS	= -std=c++0x -g #-Wall

LDFLAGS	= -L/usr/X11R6/lib -lglut -lGL -lGLU -lXi -lXmu -L/usr/x86_64-linux-gnu/libpng.so -lpng

#SRCS	= libpng/ogl_readpng.cpp UI/ui.cpp oglTexRenderer.cpp
SRCS	= libpng/loadtexture.cpp UI/ui.cpp oglTexRenderer.cpp
OBJS	= parser.tab.o lexer.yy.o $(SRCS:.cpp=.o)
PROG	= oglTexRenderer

all:	$(OBJS)
	$(CXX) -o $(PROG) $(OBJS) $(LDFLAGS) $(CXXFLAGS)

parser.tab.cpp parser.tab.hpp: parsing/parser.ypp
	bison -d parsing/parser.ypp

lexer.yy.cpp: parsing/lexer.lex
	flex -+ -o lexer.yy.cpp parsing/lexer.lex

.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -f *.o UI/*.o $(PROG) parser.tab.cpp parser.tab.hpp lexer.yy.cpp libpng/*.o
