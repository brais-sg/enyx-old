/**
 * @file AGL.h
 * @author Brais Solla González
 * @brief EnyxGL AGL base implementation
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _AGL_INCLUDED
#define _AGL_INCLUDED
#include <stdint.h>

#ifdef AGL_COLOR_16BIT
// TODO, implement for 16 bit color systems
typedef uint16_t color_t;
typedef uint32_t rgba_t;

#define R(x) ((color >> 10) & 0x1f)
#define G(x) ((color >> 5)  & 0x1f)
#define B(x) ((color >> 0)  & 0x1f)
#define A(x) ((color >> 15) & 0x01)

// 8 bit RGB to system color
#define RGB(r,g,b)      
#define RGBA(r,g,b,a)
#else
typedef uint32_t color_t;
typedef uint32_t rgba_t;

#define R(x) ((color >> 24) & 0xff)
#define G(x) ((color >> 16) & 0xff)
#define B(x) ((color >> 8)  & 0xff)
#define A(x) ((color >> 0)  & 0xff)

#define RGB(r,g,b)     (((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | 0xff)
#define RGBA(r,g,b,a)  (((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | (a & 0xff))
#endif

// Some colors
#define RED       RGB(255,0,0)
#define GREEN     RGB(0,255,0)
#define BLUE      RGB(0,0,255)
#define YELLOW    RGB(255,255,0)
#define MAGENTA   RGB(255,0,255)
#define CYAN      RGB(0,255,255)
#define GRAY      RGB(127,127,127)
#define WHITE     RGB(255,255,255)
#define BROWN     RGB(165,42,42)
#define ORANGE    RGB(255,165,0)


// AGL Interface (Adafruit GFX-Like API). All Enyx versions will have this API
class AGL {
    public:
        // Viewport settings
        virtual void viewport(int x, int y, int w, int h) = 0;
        virtual void viewport() = 0;

        // Scissor settings
        virtual void scissor(int x, int y, int w, int h) = 0;
        virtual void scissor() = 0;

        // Coordinate transformations
        virtual void origin() = 0;
        virtual void translate(float tx, float ty) = 0;
        virtual void translate(int tx, int ty) = 0;
        virtual void rotate(float angle) = 0;
        virtual void scale(float x, float y) = 0;
        virtual void scale(int x, int y) = 0;
        // Get/set transformation matrix is implemented in the renderer
        // getTransformationMatrix() / setTransformationMatrix() is renderer specific
        

        // Drawing methods
        virtual void drawPixel(int x, int y, color_t color) = 0;
        virtual void drawLine(int x0, int y0, int x1, int y1, color_t color) = 0;
        // Fast drawing functions
        virtual void drawFastVLine(int x0, int y0, int length, color_t color) = 0;
        virtual void drawFastHLine(int x0, int y0, int length, color_t color) = 0;
        
        virtual void drawRect(int x, int y, int w, int h, color_t color) = 0;
        virtual void drawFillRect(int x, int y, int w, int h, color_t color) = 0;

        virtual void drawCircle(int x, int y, int r, color_t color) = 0;
        virtual void drawFillCircle(int x, int y, int r, color_t color) = 0;

        // Not for now boy, not for now. (Maybe next AGL revision?)
        //virtual void drawRoundRect(int x, int y, int w, int h, int r, color_t color) = 0;
        //virtual void drawFillRoundRect(int x, int y, int w, int h, int r, color_t color) = 0;

        virtual void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color) = 0;
        virtual void drawFillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color) = 0;
        
        virtual void drawChar(int x, int y, char c, color_t color, uint8_t size) = 0;
        virtual void drawChar(int x, int y, char c, color_t color) = 0; // Default size

        // virtual void drawChar(uint16_t x, uint16_t y, char c, color_t color, color_t bg, uint8_t size);
        
        virtual void drawText(int x, int y, const char* text, color_t color, uint8_t size) = 0;
        virtual void drawText(int x, int y, const char* text, color_t color) = 0; // Default size

        // Clear screen methods
        virtual void clearColor(color_t color) = 0;
        virtual void clear() = 0;
        virtual void fillScreen(color_t color) = 0;

        // Screen methods
        virtual uint16_t getWidth()      const = 0;
        virtual uint16_t getHeight()     const = 0;
        virtual uint8_t  getPixelDepth() const = 0;

        // Render / submit
        virtual void submit() = 0;  // Submit buffers to GPU in hardware-accelerated contexts (Also updates matrix transform!)
        virtual void render()  = 0; // Force render (waits for renderer (glFlush & glFinish on OpenGL contexts)) 


};


















#endif