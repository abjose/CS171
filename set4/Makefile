# Makefile for cs171 HW 4

CXX		= g++
# add -g for debugging info
CXXFLAGS	= -std=c++0x -g #-Wall

LDFLAGS	= -L/usr/X11R6/lib -lglut -lGL -lGLU -lXi -lXmu

SRCS	= shaded.cpp
OBJS	= parser.tab.o lexer.yy.o $(SRCS:.cpp=.o)
PROG	= shaded

all:	$(OBJS)
	$(CXX) -o $(PROG) $(OBJS) $(LDFLAGS) $(CXXFLAGS)

parser.tab.cpp parser.tab.hpp: parsing/parser.ypp
	bison -d parsing/parser.ypp

lexer.yy.cpp: parsing/lexer.lex
	flex -+ -o lexer.yy.cpp parsing/lexer.lex

.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -f *.o $(PROG) parser.tab.cpp parser.tab.hpp lexer.yy.cpp
