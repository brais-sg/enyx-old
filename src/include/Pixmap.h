/**
 * @file Pixmap.h
 * @author Brais Solla González
 * @brief Enyx GL Pixmap
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _BITMAP_INCLUDED
#define _BITMAP_INCLUDED
#include <stdint.h>
#include "AGL.h"

class Pixmap {
    private:
        uint8_t* pm; // Pixmap
        
        uint8_t bpp;
        uint16_t width, heigth;
    public:
        Pixmap();
        Pixmap(uint16_t width, uint16_t heigth);
        Pixmap(uint16_t width, uint16_t heigth, uint8_t bpp);
        
        ~Pixmap();

        void    setPixel(uint16_t x, uint16_t y, rgba_t color);
        rgba_t  getPixel(uint16_t x, uint16_t y);
        uint16_t getWidth()  const;
        uint16_t getHeigth() const;
        uint8_t  getBpp()    const;

        void* getPixmap();
};

#endif