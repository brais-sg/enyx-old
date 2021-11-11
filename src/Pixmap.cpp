/**
 * @file Pixmap.cpp
 * @author Brais Solla González
 * @brief Pixmap class implementation for Enyx (Rewritten)
 * @version 0.1
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "AGL.h"
#include "Pixmap.h"
#include "Debug.h"
#include "ImageDriver.h"


Pixmap::Pixmap(){
    this->px = NULL;
    this->width      = 0;
    this->height     = 0;
    this->components = 0;
    this->loader     = PIXMAP_LOADER_NONE;
}

Pixmap::Pixmap(const char* fileName){
    this->px = ImageDriver::loadImage(fileName, &this->width, &this->height, &this->components);
    if(this->px){
        Debug::info("[%s:%d]: Loaded image %s with size (%dx%dx%d)\n", __FILE__, __LINE__, fileName, this->width, this->height, this->components);
        this->loader = PIXMAP_LOADER_IMAGEDRIVER;
    } else {
        Debug::error("[%s:%d]: Cannot load image %s!\n", __FILE__, __LINE__, fileName);

        this->px = NULL;
        this->width      = 0;
        this->height     = 0;
        this->components = 0;
        this->loader     = PIXMAP_LOADER_NONE;
    }
}

Pixmap::Pixmap(const Pixmap& other){
    this->px = (uint8_t*) malloc(other.getWidth() * other.getHeight() * other.getComponents());
    if(this->px){
        Debug::info("[%s:%d]: Loading image from another Pixmap (copy constructor) from %p\n", __FILE__, __LINE__, &other);

        this->width      = other.getWidth();
        this->height     = other.getHeight();
        this->components = other.getComponents();

        memcpy(this->px, other.getPixels(), this->width * this->height * this->components);

        this->loader     = PIXMAP_LOADER_MALLOC;
    } else {
        Debug::error("[%s:%d]: Cannot allocate memory to copy Pixmap!\n", __FILE__, __LINE__);

        this->px = NULL;
        this->width      = 0;
        this->height     = 0;
        this->components = 0;
        this->loader     = PIXMAP_LOADER_NONE;
    }
}


Pixmap::~Pixmap(){
    if(this->loader != PIXMAP_LOADER_NONE){
        this->free();
    } else {
        this->px = NULL;
    }
}

bool Pixmap::onImage(int px, int py) const {
    if(px >= 0 && px < this->width && py >= 0 && py < this->height) return true;
    return false;
}

bool Pixmap::isModifiable() const {
    return (this->loader != PIXMAP_LOADER_NONE);
}


Pixmap::operator bool() const {
    return (this->px != NULL);
}

void* Pixmap::getPixels() const {
    return (void*) this->px;
}

void Pixmap::free(){
    if(this->loader != PIXMAP_LOADER_NONE){
        if(this->loader == PIXMAP_LOADER_IMAGEDRIVER){
            Debug::info("[%s:%d]: Deleting an ImageDriver allocated pixmap (%p)\n", __FILE__, __LINE__, this->px);
            ImageDriver::freeImage(this->px);
        } else {
            Debug::info("[%s:%d]: Deleting a locally allocated pixmap (%p)\n", __FILE__, __LINE__, this->px);
            ::free(this->px);
        }
    } else {
        Debug::info("[%s:%d]: Deleting a static buffer pixmap (%p)\n", __FILE__, __LINE__, this->px);
    }

    this->px = NULL;
    this->width      = 0;
    this->height     = 0;
    this->components = 0;
    this->loader     = PIXMAP_LOADER_NONE;
}

void Pixmap::allocate(int width, int height, int components){
    if(this) this->free();

    this->px = (uint8_t*) malloc(width * height * components);
    if(this->px){
        Debug::info("[%s:%d]: Allocated %d bytes for image\n", width * height * components);
        
        this->width      = width;
        this->height     = height;
        this->components = components;
        this->loader     = PIXMAP_LOADER_MALLOC;

        memset(this->px, 0, width * height * components);
    } else {
        Debug::error("[%s:%d]: Cannot allocate memory to image (%d bytes)!\n", __FILE__, __LINE__, width * height * components);

        this->px = NULL;
        this->width      = 0;
        this->height     = 0;
        this->components = 0;
        this->loader     = PIXMAP_LOADER_NONE;
    }
}

void Pixmap::clear(){
    if(this && this->isModifiable()){
        memset(this->px, 0, this->width * this->height * this->components);
    }
}

void Pixmap::fill(color_t color){
    if(this && this->isModifiable()){
        switch(this->components){
            case 1:
                memset(this->px, R(color), this->width * this->height * this->components);
                break;
            case 2:
                for(int i = 0; i < (this->width * this->height * this->components); i+=2){
                    this->px[i + 0] = R(color);
                    this->px[i + 1] = G(color);
                }
                break;
            case 3:
                for(int i = 0; i < (this->width * this->height * this->components); i+=3){
                    this->px[i + 0] = R(color);
                    this->px[i + 1] = G(color);
                    this->px[i + 2] = B(color);
                }
                break;
            case 4:
                for(int i = 0; i < (this->width * this->height * this->components); i+=4){
                    this->px[i + 0] = R(color);
                    this->px[i + 1] = G(color);
                    this->px[i + 2] = B(color);
                    this->px[i + 3] = A(color);
                }
                break;
            default:
                Debug::warning("[%s:%d]: Aborting fill operation. Unknown image components: %d\n", this->components);
                break;
        }
    } else {
        if(!this->isModifiable()) Debug::error("[%s:%d]: Attempting to modify a static pixmap!\n", __FILE__, __LINE__);
    }
}


int Pixmap::flipHorizontally(){
    if(this && this->isModifiable()){



    } else {
        if(!this->isModifiable()) Debug::error("[%s:%d]: Attempting to modify a static pixmap!\n", __FILE__, __LINE__);
    }
}




color_t Pixmap::getPixel(int px, int py){
    if(this){
        if(onImage(px, py)){
            // Image is stored in big endian mode [R G B A]
            int offset = (this->components * this->width * py) + (this->components * px);
            switch(this->components){
                case 1:
                    return RGB(this->px[offset + 0], 0, 0);
                    break;
                case 2:
                    return RGB(this->px[offset + 0], this->px[offset + 1], 0);
                    break;
                case 3:
                    return RGB(this->px[offset + 0], this->px[offset + 1], this->px[offset + 2]);
                    break;
                case 4:
                    return RGBA(this->px[offset + 0], this->px[offset + 1], this->px[offset + 2], this->px[offset + 3]);
                    break;
                default:
                    return 0;
            }
        } else {
            return 0;
        }
    }
    return 0;
}


void Pixmap::setPixel(int px, int py, color_t color){
    if(this && this->isModifiable()){
        



    } else {
        if(!this->isModifiable()) Debug::error("[%s:%d]: Attempting to modify a static pixmap!\n", __FILE__, __LINE__);
    }
}