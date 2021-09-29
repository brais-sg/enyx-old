# Enyx Graphics Library Makefile
# Development version
# By Brais Solla González

CC   = g++ -O0
LIBS = -lm -lSDL2 -lGLESv2
EXEC = Enyx
SRC  = src

#$(SRC)/%.o: %.cpp $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS) -Iinclude
all: Pixmap.o WindowSDL2.o MouseSDL2.o Enyx

WindowSDL2.o: src/WindowSDL2.cpp
	$(CC) -c src/WindowSDL2.cpp -I./src/include/ $(LIBS)
MouseSDL2.o:  src/MouseSDL2.cpp
	$(CC) -c src/MouseSDL2.cpp -I./src/include/ $(LIBS)
Pixmap.o: src/Pixmap.cpp
	$(CC) -c src/Pixmap.cpp -I./src/include/ $(LIBS)

Enyx: src/Test.cpp
	$(CC) -o $(EXEC) src/Test.cpp -I./src/include *.o $(LIBS)

clean:
	rm -rf *.o $(EXEC)