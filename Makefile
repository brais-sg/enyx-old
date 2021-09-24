# Enyx Graphics Library Makefile
# Development version
# By Brais Solla González

CC   = g++ -O0
LIBS = -lm -lSDL2 -lGLESv2
EXEC = Enyx
SRC  = src

#$(SRC)/%.o: %.cpp $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS) -Iinclude
all: Pixmap.o Enyx

Pixmap.o: src/Pixmap.cpp
	$(CC) -c src/Pixmap.cpp -I./src/include/ $(LIBS)
Enyx: src/Test.cpp
	$(CC) -o $(EXEC) src/Test.cpp -I./src/include *.o $(LIBS)

clean:
	rm -rf *.o $(EXEC)