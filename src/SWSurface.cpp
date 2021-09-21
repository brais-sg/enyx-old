/**
 * @file SWSurface.cpp
 * @author Brais Solla González
 * @brief Enyx GL Software Surface implementation
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>

#include "AGL.h"
#include "SWSurface.h"

#ifndef abs
#define abs(x) (((x) > 0) ? (x) : (-(x)))
#endif

#ifndef _swap_int
#define _swap_int(a, b)                                                    \
  {                                                                            \
    int _tmp = a;                                                             \
    a = b;                                                                     \
    b = _tmp;                                                                     \
  }
#endif

SWSurface::SWSurface(){
    this->pm = NULL;
    this->width  = 0;
    this->height = 0;
}

SWSurface::SWSurface(uint16_t width, uint16_t height){
    this->pm = (color_t*) malloc(width * height * COLOR_DEPTH);
    if(this->pm){
        this->width  = width;
        this->height = height;
    }
}

SWSurface::~SWSurface(){
    if(this->pm) free(this->pm);
}

void* SWSurface::getPixmap(){
    return (void*) this->pm;
}

uint16_t SWSurface::getWidth() const {
    return this->width;
}

uint16_t SWSurface::getHeight() const {
    return this->height;
}

uint8_t SWSurface::getPixelDepth() const {
    return COLOR_DEPTH;  
}


void SWSurface::drawPixel(int x, int y, color_t color){
    // Check if pixel is in surface area

    this->pm[y * width + x] = color;
}

void SWSurface::drawLine(int x0, int y0, int x1, int y1, color_t color){
    // Bresenham algorithm
    // LOOKAT: https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp#L132
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if(steep){
        _swap_int(x0,x1);
        _swap_int(x1,y1);
    }

    if(x0 > x1){
        _swap_int(x0, x1);
        _swap_int(y0, y1);
    }

    int dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int err = dx >> 1;
    int ystep;

    if(y0 < y1){
        ystep = 1;
    } else {
        ystep = -1;
    }

    while(x0 <= x1){
        if(steep){
            this->drawPixel(y0, x0, color);
        } else {
            this->drawPixel(x0, y0, color);
        }
        err -= dy;
        if(err < 0){
            y0  += ystep;
            err += dx;
        }

        x0++;
    }
}

void SWSurface::drawFastVLine(int x0, int y0, int length, color_t color){
    for(int offset = 0; offset < length; offset++){
        this->drawPixel(x0, y0 + offset, color);
    }
}

void SWSurface::drawFastHLine(int x0, int y0, int length, color_t color){
    for(int offset = 0; offset < length; offset++){
        this->drawPixel(x0 + offset, y0, color);
    }
}