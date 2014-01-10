# Makefile for cs171 HW 8

CXX		= g++
# add -g for debugging info
CXXFLAGS	= -std=c++0x -g #-Wall

LDFLAGS	= -L/usr/lib/nvidia -L/usr/X11R6/lib -lglut -lGL -lGLU -lXi -lXmu -L/usr/x86_64-linux-gnu/libpng.so -lpng

SRCS	= readpng.cpp main.cpp
OBJS	= $(SRCS:.cpp=.o)
PROG	= glslRenderer

all:	$(OBJS)
	$(CXX) -o $(PROG) $(OBJS) $(LDFLAGS) $(CXXFLAGS)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -f *.o $(PROG)
