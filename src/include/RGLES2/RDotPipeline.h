/**
 * @file RDotPipeline.h
 * @author Brais Solla González
 * @brief RDotPipeline Definition
 * @version 0.1
 * @date 2021-11-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RDOTPIPELINE_INCLUDED
#define _ENYX_RGLES2_RDOTPIPELINE_INCLUDED

#include "RGLES2/RPipeline.h"
#include "RGLES2/RShader.h"

class RDotPipeline : public RPipeline {
    private:
        RShader* internalShader;
    public:
        RDotPipeline();
        ~RDotPipeline();

        void enable();
        void disable();
        void setTransform(RMatrix4& matrix);
        void draw(void* buffer);

        void setPointSize(int pointSize);
};

#endif