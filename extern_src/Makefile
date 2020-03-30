SOURCES=$(shell echo *.cpp)

OBJDIR=obj
OBJECTS = $(SOURCES:.cpp=.o)

GL_LIBS	= $(shell pkg-config --libs gl glew)
GL_CFLAGS = $(shell pkg-config --cflags gl glew)

CXXFLAGS=-Wall -march=native -std=c++17 -O3 $(GL_CFLAGS) # -gdwarf-3 -g3
LDFLAGS=$(GL_LIBS)

YUV420P_Player.o: YUV420P_Player.cpp
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f $(TARGET)
