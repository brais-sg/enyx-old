/**
 * @file RTrianglePipeline.h
 * @author Brais Solla González
 * @brief RTrianglePipeline
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RTRIANGLEPIPELINE_INCLUDED
#define _ENYX_RGLES2_RTRIANGLEPIPELINE_INCLUDED

#include "RGLES2/RPipeline.h"
#include "RGLES2/RShader.h"

class RTrianglePipeline : public RPipeline {
    private:
        RShader* internalShader;
    public:
        RTrianglePipeline();
        ~RTrianglePipeline();

        void enable();
        void disable();
        void setTransform(RMatrix4& matrix);
        void draw(void* buffer);
};

#endif