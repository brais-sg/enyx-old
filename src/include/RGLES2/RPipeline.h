/**
 * @file RPipeline.h
 * @author Brais Solla González
 * @brief Rendering Pipeline abstract class / interface definition
 * @version 0.1
 * @date 2021-11-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RPIPELINE_INCLUDED
#define _ENYX_RGLES2_RPIPELINE_INCLUDED

#include "RGLES2/RMatrix3.h"
#include "RGLES2/RMatrix4.h"

class RPipeline {
    public:
        virtual ~RPipeline();
        // Enable rendering pipeline (Only on context change)
        virtual void enable()  = 0;
        // Disable rendering pipeline (Only on context change)
        virtual void disable() = 0;
        // Draw method (Receives pointer to auxiliary buffer)
        virtual void draw(void* buffer) = 0;

        // Set uniforms!
        virtual void setTransform(RMatrix4& matrix)  = 0;
};


#endif