/**
 * @file SWSurface.h
 * @author Brais Solla González
 * @brief Enyx GL software surface
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef SWSURFACE_INCLUDED
#define SWSURFACE_INCLUDED

#include "AGL.h"

// Color depth in bytes 
#ifdef AGL_COLOR_16BIT
#define COLOR_DEPTH 4
#else
#define COLOR_DEPTH 2
#endif

class SWSurface : public AGL {
    private:
        color_t* pm;
        uint16_t width;
        uint16_t height;
    
    public:
        SWSurface();
        SWSurface(uint16_t width, uint16_t height);
        ~SWSurface();

        void*    getPixmap();

        uint16_t getWidth()  const;
        uint16_t getHeight() const;
        uint8_t getPixelDepth() const;
        
        // Drawing functions
        void drawPixel(uint16_t x, uint16_t y, color_t color);
        void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color);
        
        void drawFastVLine(uint16_t x0, uint16_t y0, uint16_t length, color_t color);
        void drawFastHLine(uint16_t x0, uint16_t y0, uint16_t length, color_t color);


};


#endif