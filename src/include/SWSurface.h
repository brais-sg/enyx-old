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

#ifdef AGL_COLOR_16BIT
#define COLOR_DEPTH 4
#else
#define COLOR_DEPTH 2
#endif

class SWSurface : public AGL {
    private:
        uint8_t* pm;
        uint16_t width;
        uint16_t heigth;
    
    public:
        SWSurface();
        SWSurface(uint16_t width, uint16_t heigth);




};


#endif