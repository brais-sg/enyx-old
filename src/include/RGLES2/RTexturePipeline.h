/**
 * @file RTexturePipeline.h
 * @author Brais Solla González
 * @brief RTexturePipeline
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RTEXTUREPIPELINE_INCLUDED
#define _ENYX_RGLES2_RTEXTUREPIPELINE_INCLUDED

#include "RGLES2/RPipeline.h"
#include "RGLES2/RShader.h"

class RTexturePipeline : public RPipeline {
    private:
        RShader* internalShader;
    public:
        RTexturePipeline();
        ~RTexturePipeline();

        void enable();
        void disable();
        void setTransform(RMatrix4& matrix);
        void draw(void* buffer);
};


#endif