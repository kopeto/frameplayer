TARGET=player

SOURCES=$(shell echo *.cpp)

OBJDIR=obj
OBJ_FILES = $(SOURCES:.cpp=.o)
OBJECTS = $(addprefix obj/, $(OBJ_FILES))

# AV_LIBS	= $(shell pkg-config --libs libavformat libavcodec libavutil libswscale)
# AV_CFLAGS = $(shell pkg-config --cflags libavformat libavcodec libavutil libswscale)

# SDL_LIBS = $(shell sdl2-config --libs)
# SDL_CFLAGS = $(shell sdl2-config --cflags)

SDL_CFLAGS = -I./include/SDL2 
AV_CFLAGS = -I./include

SDL_LIBS = -lSDL2
AV_LIBS = -lavformat -lavcodec -lavutil -lswscale

CXXFLAGS=-Wall -march=native -std=c++17 -O3 $(AV_CFLAGS) $(SDL_CFLAGS)
LDFLAGS= -L./lib $(AV_LIBS) $(SDL_LIBS) -pthread

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -g $(OBJECTS) $(LDFLAGS) -o $(TARGET)

$(OBJDIR)/%.o: %.cpp
	g++ -g $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o
	rm -f *.o
	rm -f $(TARGET)
