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
        // Drawing methods
        virtual void drawPixel(uint16_t x, uint16_t y, color_t color) = 0;
        virtual void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color) = 0;
        // Fast drawing functions
        virtual void drawFastVLine(uint16_t x0, uint16_t y0, uint16_t length, color_t color) = 0;
        virtual void drawFastHLine(uint16_t x0, uint16_t y0, uint16_t length, color_t color) = 0;
        
        virtual void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, color_t color) = 0;
        virtual void drawFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, color_t color) = 0;

        virtual void drawCircle(uint16_t x, uint16_t y, uint16_t r, color_t color) = 0;
        virtual void drawFillCircle(uint16_t x, uint16_t y, uint16_t r, color_t color) = 0;

        virtual void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, color_t color) = 0;
        virtual void drawFillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, color_t color) = 0;

        virtual void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, color_t color) = 0;
        virtual void drawFillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, color_t color) = 0;
        
        virtual void drawChar(uint16_t x, uint16_t y, char c, color_t color, uint8_t size) = 0;
        virtual void drawChar(uint16_t x, uint16_t y, char c, color_t color) = 0; // Default size

        // virtual void drawChar(uint16_t x, uint16_t y, char c, color_t color, color_t bg, uint8_t size);
        
        virtual void drawText(uint16_t x, uint16_t y, const char* text, color_t color, uint8_t size) = 0;
        virtual void drawText(uint16_t x, uint16_t y, const char* text, color_t color) = 0; // Default size

        virtual void clearColor(color_t color) = 0;
        virtual void clear() = 0;
        virtual void fillScreen(color_t color) = 0;

        // Screen methods
        virtual uint16_t getWidth()      const = 0;
        virtual uint16_t getHeight()     const = 0;
        virtual uint8_t  getPixelDepth() const = 0;
};


















#endif