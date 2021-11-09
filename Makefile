# Enyx Graphics Library Makefile
# Development version
# By Brais Solla González

CC     = g++ -O0 -I./src/include
CFLAGS = -Wall -g 
LIBS   = -lm -lSDL2 -lGLESv2
TARGET = Enyx

all: Debug.o ImageDriver.o Pixmap.o Platform_SDL2.o RGLES2.a $(TARGET)

Debug.o:
	$(CC) $(CFLAGS) -c src/Debug.cpp
ImageDriver.o:
	$(CC) $(CFLAGS) -c src/ImageDriver.cpp
Pixmap.o:
	$(CC) $(CFLAGS) -c src/Pixmap.cpp
Platform_SDL2.o:
	$(CC) $(CFLAGS) -c src/Platform_SDL2.cpp

RGLES2.a: RGLES2.o
	ar rc librgles2.a RGLES2.o
RGLES2.o:
	$(CC) $(CFLAGS) -c src/RGLES2/RGLES2.cpp

$(TARGET): Debug.o ImageDriver.o Pixmap.o Platform_SDL2.o RGLES2.a
	$(CC) $(CFLAGS) -o $(TARGET) src/Test.cpp *.o *.a $(LIBS)

clean:
	rm -rf *.o *.a $(TARGET)

.PHONY: all clean
