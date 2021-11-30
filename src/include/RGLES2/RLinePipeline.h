/**
 * @file RLinePipeline.h
 * @author Brais Solla González
 * @brief RLinePipeline
 * @version 0.1
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RLINEPIPELINE_INCLUDED
#define _ENYX_RGLES2_RLINEPIPELINE_INCLUDED

#include "RGLES2/RPipeline.h"
#include "RGLES2/RShader.h"

class RLinePipeline : public RPipeline {
    private:
        RShader* internalShader;
    public:
        RLinePipeline();
        ~RLinePipeline();

        void enable();
        void disable();
        void setTransform(RMatrix4& matrix);
        void draw(void* buffer);
};

#endif