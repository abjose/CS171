# Makefile for cs171 HW 5

CXX		= g++
# add -g for debugging info
CXXFLAGS	= -std=c++0x -g #-Wall

LDFLAGS	= -L/usr/X11R6/lib -lglut -lGL -lGLU -lXi -lXmu

SRCS	= nurbs/spline.cpp UI/ui.cpp editSpline.cpp
OBJS	= $(SRCS:.cpp=.o)
PROG	= editSpline

all:	$(OBJS)
	$(CXX) -o $(PROG) $(OBJS) $(LDFLAGS) $(CXXFLAGS)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -f *.o nurbs/*.o UI/*.o $(PROG)
