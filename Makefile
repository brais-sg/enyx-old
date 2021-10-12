# Enyx Graphics Library Makefile
# Development version
# By Brais Solla González

CC   = g++ -O0 -g
LIBS = -lm -lSDL2 -lGLESv2
EXEC = Enyx
SRC  = src

#$(SRC)/%.o: %.cpp $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS) -Iinclude
all: Pixmap.o ImageDriver.o Platform_SDL2.o Renderer_SDL2_GLES2.o Enyx

Pixmap.o: src/Pixmap.cpp
	$(CC) -c src/Pixmap.cpp -I./src/include/ $(LIBS)

ImageDriver.o: src/ImageDriver.cpp
	$(CC) -c src/ImageDriver.cpp -I./src/include/ $(LIBS)
Platform_SDL2.o: src/Platform_SDL2.cpp
	$(CC) -c src/Platform_SDL2.cpp -I./src/include/ $(LIBS)
Renderer_SDL2_GLES2.o: src/Renderer_SDL2_GLES2.cpp
	$(CC) -c src/Renderer_SDL2_GLES2.cpp -I./src/include/ $(LIBS)

Enyx: src/Test.cpp
	$(CC) -o $(EXEC) src/Test.cpp -I./src/include *.o $(LIBS)

clean:
	rm -rf *.o $(EXEC)
