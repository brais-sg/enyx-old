# Enyx Graphics Library Makefile
# Development version
# By Brais Solla González

CC     = g++ -O0 -I./src/include
CFLAGS = -Wall -g 
LIBS   = -lm -lSDL2 -lGLESv2
TARGET = Enyx

all: Debug.o ImageDriver.o Pixmap.o Platform_SDL2.o RGLES2.o $(TARGET)

Debug.o:
	$(CC) $(CFLAGS) -c src/Debug.cpp
ImageDriver.o:
	$(CC) $(CFLAGS) -c src/ImageDriver.cpp
Pixmap.o:
	$(CC) $(CFLAGS) -c src/Pixmap.cpp
Platform_SDL2.o:
	$(CC) $(CFLAGS) -c src/Platform_SDL2.cpp

# RGLES2 objects
RMatrix4.o:
	$(CC) $(CFLAGS) -c src/RGLES2/RMatrix4.cpp
RMatrix3.o:
	$(CC) $(CFLAGS) -c src/RGLES2/RMatrix3.cpp
RVector2i.o:
	$(CC) $(CFLAGS) -c src/RGLES2/RVector2i.cpp
RVector2.o:
	$(CC) $(CFLAGS) -c src/RGLES2/RVector2.cpp
RShader.o:
	$(CC) $(CFLAGS) -c src/RGLES2/RShader.cpp
RDotPipeline.o:
	$(CC) $(CFLAGS) -c src/RGLES2/RDotPipeline.cpp
RLinePipeline.o:
	$(CC) $(CFLAGS) -c src/RGLES2/RLinePipeline.cpp
RTrianglePipeline.o:
	$(CC) $(CFLAGS) -c src/RGLES2/RTrianglePipeline.cpp

#RGLES2.a: RGLES2.o RMatrix4.o RMatrix3.o RVector2i.o RVector2.o RShader.o RDotPipeline.o
#	ar rc librgles2.a RGLES2.o RMatrix4.o RMatrix3.o RVector2i.o RVector2.o RShader.o RDotPipeline.o

RGLES2.o: RMatrix4.o RMatrix3.o RVector2i.o RVector2.o RShader.o RDotPipeline.o RLinePipeline.o RTrianglePipeline.o
	$(CC) $(CFLAGS) -c src/RGLES2/RGLES2.cpp


# Final target. TODO: Build .a library before!
$(TARGET): Debug.o ImageDriver.o Pixmap.o Platform_SDL2.o RGLES2.o
	$(CC) $(CFLAGS) -o $(TARGET) src/Test.cpp *.o $(LIBS)

clean:
	rm -rf *.o *.a $(TARGET)

.PHONY: all clean
