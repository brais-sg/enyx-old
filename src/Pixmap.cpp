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
#include "Pixmap.h"

Pixmap::Pixmap(){
    this->pm = NULL;
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

}

rgba_t Pixmap::getPixel(uint16_t x, uint16_t y){

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
