TARGET=player

SOURCES=$(shell echo *.cpp)

OBJDIR=obj
OBJ_FILES = $(SOURCES:.cpp=.o)
OBJECTS = $(addprefix obj/, $(OBJ_FILES))

# AV_LIBS	= $(shell pkg-config --libs libavformat libavcodec libavutil libswscale)
# AV_CFLAGS = $(shell pkg-config --cflags libavformat libavcodec libavutil libswscale)

AV_LIBS = -lavformat -lavcodec -lavutil -lswscale
AV_CFLAGS =

SDL_LIBS = -lSDL2
SDL_CFLAGS = -I./include/SDL2/

CXXFLAGS=-Wall -march=native -std=c++17 -O3 -I./include $(AV_CFLAGS) $(SDL_CFLAGS)
LDFLAGS= -L./lib $(AV_LIBS) $(SDL_LIBS)


all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) $(LDFLAGS) -o $(TARGET)

$(OBJDIR)/%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o
	rm -f *.o
	rm -f $(TARGET)
