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


void SWSurface::drawPixel(uint16_t x, uint16_t y, color_t color){
    this->pm[y * width + x] = color;
}

void SWSurface::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color){
    // Bresenham algorithm
    // LOOKAT: Adafruit GFX GitHub
}

void SWSurface::drawFastVLine(uint16_t x0, uint16_t y0, uint16_t length, color_t color){
    for(uint16_t offset = 0; offset < length; offset++){
        this->drawPixel(x0, y0 + offset, color);
    }
}

void SWSurface::drawFastHLine(uint16_t x0, uint16_t y0, uint16_t length, color_t color){
    for(uint16_t offset = 0; offset < length; offset++){
        this->drawPixel(x0 + offset, y0, color);
    }
}