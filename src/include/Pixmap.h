/**
 * @file Pixmap.h
 * @author Brais Solla González
 * @brief Pixmap class implementation for Enyx (Rewritten)
 * @version 0.1
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _PIXMAP_INCLUDED
#define _PIXMAP_INCLUDED
#include <stdint.h>
#include "AGL.h"


enum PIXMAP_LOADER {
    PIXMAP_LOADER_NONE        = 0,
    PIXMAP_LOADER_MALLOC      = 1,
    PIXMAP_LOADER_IMAGEDRIVER = 2
};

class Pixmap {
    private:
        uint8_t* px;
        int width, height, components, loader;
    public:
        Pixmap();
        // Constructor for static array! Do not use
        Pixmap(void* px, int width, int height, int components, int loader);
        // Load from image (ImageDriver)
        Pixmap(const char* fileName);
        Pixmap(const Pixmap& other);
        ~Pixmap();

        bool onImage(int px, int py) const;
        bool isModifiable()          const;
        
        bool exists() const;

        // Get pixel array
        void* getPixels() const;

        // Copy pixels to array
        void copyPixels(void* from, size_t size);

        // Free image storage (Does nothing on static)
        void free();

        // Allocate image storage
        void allocate(int width, int height, int components);

        // Clear image
        void clear();

        // Fill image
        void fill(color_t color);

        // Only allowed with dynamic storage.
        void flipHorizontally();
        void flipVertically();

        color_t getPixel(int px, int py);
        void    setPixel(int px, int py, color_t color);

        int getWidth()      const;
        int getHeight()     const;
        int getComponents() const;
        int getDepth()      const;
        int getBpp()        const;


        static Pixmap loadImage(const char* fileName);
        static Pixmap loadArray(void* px_ptr, int width, int height, int components);
        static Pixmap loadStaticArray(void* px_ptr, int width, int height, int components);
        static void   saveImage(const char* fileName, const Pixmap& pixmap);
};

#endif