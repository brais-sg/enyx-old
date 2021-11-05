/**
 * @file Pixmap.cpp
 * @author Brais Solla González
 * @brief Enyx GL Pixmap implementation
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "Pixmap.h"
#include "ImageDriver.h"

Pixmap::Pixmap(){
    this->pm     = NULL;
    this->width  = 0;
    this->height = 0;
    this->bpp    = 0;
}

Pixmap::Pixmap(uint16_t width, uint16_t heigth, uint8_t bpp){
    this->pm = (uint8_t*) malloc(width * heigth * bpp);
    if(this->pm){
        this->width  = width;
        this->height = heigth;
        this->bpp    = bpp;
    }
}

Pixmap::Pixmap(uint16_t width, uint16_t heigth){
    this->pm = (uint8_t*) malloc(width * heigth * 4);
    if(this->pm){
        this->width  = width;
        this->height = heigth;
        this->bpp    = bpp;
    }
}

Pixmap::~Pixmap(){
    if(this->pm) free(this->pm);
}


void Pixmap::setPixel(uint16_t x, uint16_t y, rgba_t color){
    uint32_t px_offset = (y * this->bpp * this->width) + (x * this->bpp);

    if(x >= 0 && x < this->width && y >= 0 && y < this->height){
        switch(this->bpp){
            case 4:
                this->pm[px_offset + 3] = A(color);
            case 3:
                this->pm[px_offset + 2] = B(color);
            case 2:
                this->pm[px_offset + 1] = G(color);
            case 1:
                this->pm[px_offset + 0] = R(color);
                break;
            default:
                return;
        }
    }
}

rgba_t Pixmap::getPixel(uint16_t x, uint16_t y){
    uint32_t px_offset = (y * this->bpp * this->width) + (x * this->bpp);
    color_t color = 0;

    if(x >= 0 && x < this->width && y >= 0 && y < this->height){
        switch(this->bpp){
            case 4:
                color |= this->pm[px_offset + 3] << 24;
            case 3:
                color |= this->pm[px_offset + 2] << 16;
            case 2:
                color |= this->pm[px_offset + 1] << 8;
            case 1:
                color |= this->pm[px_offset + 0] << 0;
                break;
            default:
                break;
        }
    }

    return color;
}

void Pixmap::clear(){
    if(this->pm) memset(this->pm, 0, width * height * bpp);
}

void* Pixmap::getPixmap(){
    return (void*) this->pm;
}

uint16_t Pixmap::getWidth() const {
    return this->width;
}

uint16_t Pixmap::getHeight() const {
    return this->height;
}

uint8_t Pixmap::getBpp() const {
    return this->bpp;
}
