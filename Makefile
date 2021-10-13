# Enyx Graphics Library Makefile
# Development version
# By Brais Solla González

CC     = g++ -O0 -I./src/include
CFLAGS = -Wall -g 
LIBS   = -lm -lSDL2 -lGLESv2
TARGET = Enyx
SRC    = .../src
OBJ    = .../obj

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
%.o: %.cpp
	$(CC) $(CFLAGS) $(LIBS) -c $<

clean:
	rm -rf *.o $(TARGET)

.PHONY: all clean
