# compiling variables
CXXFLAGS = -std=c++20 -O2 -fPIC
CPPFLAGS = -DNDEBUG -Iinclude
LDFLAGS = -L. -Wl,-rpath=${PWD}
LDLIBS = 

# source file
SRCS = $(wildcard src/*.cpp)
# object file from sources
OBJS = $(SRCS:.cpp=.o)
# all headers
HAEDS =$(wildcard *.hpp)
# exe main
EXEC = main

INCLUDE_DIR = include

.phony = all clean 

#all : $(SRCS) $(OBJS) $(EXEC)
all: $(EXEC)

$(EXEC) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS) $(CPPFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I$(INCLUDE_DIR)

clean:
	rm -f $(OBJS) $(EXEC)