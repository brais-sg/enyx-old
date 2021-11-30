/**
 * @file RBasicTexturePipeline.h
 * @author Brais Solla González
 * @brief RBasicTexturePipeline
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RBASICTEXTUREPIPELINE_INCLUDED
#define _ENYX_RGLES2_RBASICTEXTUREPIPELINE_INCLUDED

#include "RGLES2/RPipeline.h"
#include "RGLES2/RShader.h"

class RBasicTexturePipeline : public RPipeline {
    private:
        RShader* internalShader;
    public:
        RBasicTexturePipeline();
        ~RBasicTexturePipeline();

        void enable();
        void disable();
        void draw(void* buffer);
};


#endif