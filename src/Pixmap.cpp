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

Pixmap::Pixmap(void* px, int width, int height, int components, int loader){
    this->px         = (uint8_t*) px;
    this->width      = width;
    this->height     = height;
    this->components = components;
    this->loader     = loader;
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

void Pixmap::copyPixels(void* from, size_t size){
    if(this && this->isModifiable()){
        memcpy(this->px, from, size);
    } else {
        if(!this->isModifiable()) Debug::error("[%s:%d]: Attempting to modify a static pixmap!\n", __FILE__, __LINE__);
    }
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


void Pixmap::flipHorizontally(){
    if(this && this->isModifiable()){
        for(int y = 0; y < (this->height >> 1); y++){
            for(int x = 0; x < this->width; x++){
                color_t px1 = this->getPixel(x, y);
                color_t px2 = this->getPixel(x, this->height - y);

                this->setPixel(x, y, px2);
                this->setPixel(x, this->height - y, px1);
            }
        }
    } else {
        if(!this->isModifiable()) Debug::error("[%s:%d]: Attempting to modify a static pixmap!\n", __FILE__, __LINE__);
    }
}

void Pixmap::flipVertically(){
    if(this && this->isModifiable()){
        for(int y = 0; y < this->height; y++){
            for(int x = 0; x < (this->width >> 1); x++){
                color_t px1 = this->getPixel(x, y);
                color_t px2 = this->getPixel(this->width - x, this->height);

                this->setPixel(x, y, px2);
                this->setPixel(this->width - x, y, px1);
            }
        }
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
        int offset = (this->components * this->width * py) + (this->components * px);
        switch(this->components){
            case 1:
                this->px[offset + 0] = R(color);
                break;
            case 2:
                this->px[offset + 0] = R(color);
                this->px[offset + 1] = G(color);
                break;
            case 3:
                this->px[offset + 0] = R(color);
                this->px[offset + 1] = G(color);
                this->px[offset + 2] = B(color);
                break;
            case 4:
                this->px[offset + 0] = R(color);
                this->px[offset + 1] = G(color);
                this->px[offset + 2] = B(color);
                this->px[offset + 3] = A(color);
                break;
            default:
                break;
        }
    } else {
        if(!this->isModifiable()) Debug::error("[%s:%d]: Attempting to modify a static pixmap!\n", __FILE__, __LINE__);
    }
}

int Pixmap::getWidth() const {
    return this->width;
}

int Pixmap::getHeight() const {
    return this->height;
}

int Pixmap::getComponents() const {
    return this->components;
}

int Pixmap::getDepth() const {
    return this->components * 8;
}

// This method is here for legacy purposes.
// This will be deprecated. Use getDepth instead
int Pixmap::getBPP() const {
    return this->getDepth();
}


// Load image
Pixmap Pixmap::loadImage(const char* fileName){
    return Pixmap(fileName);
}

Pixmap Pixmap::loadArray(void* px_ptr, int width, int height, int components){
    Pixmap pixmap;
    Debug::info("[%s:%d]: Allocating a pixmap from array %p\n", __FILE__, __LINE__, px_ptr);
    pixmap.allocate(width, height, components);
    pixmap.copyPixels(px_ptr, width * height * components);

    return pixmap;
}

Pixmap Pixmap::loadStaticArray(void* px_ptr, int width, int height, int components){
    Debug::info("[%s:%d]: Allocating a static pixmap from array %p\n", __FILE__, __LINE__, px_ptr);
    Pixmap pixmap(px_ptr, width, height, components, PIXMAP_LOADER_NONE);
    return pixmap;
}

void Pixmap::saveImage(const char* fileName, const Pixmap& pixmap){
    Debug::info("[%s:%d]: Saving image to file %s\n", fileName);
    if(ImageDriver::writeImage(fileName, pixmap.getWidth(), pixmap.getHeight(), pixmap.getComponents(), pixmap.getPixels())){
        Debug::error("[%s:%d]: Cannot save image to file %s\n", __FILE__, __LINE__, fileName);
    }
}

