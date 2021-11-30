/**
 * @file RShader.h
 * @author Brais Solla González
 * @brief RGLES2 renderer shader implementation (OpenGL ES 2.0 ONLY)
 * @version 0.1
 * @date 2021-11-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RSHADER_INCLUDED
#define _ENYX_RGLES2_RSHADER_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include <GLES2/gl2.h>

class RShader {
    private:
        GLuint programId;

        GLint vertex_attrib;
        GLint color_attrib;
        GLint texcoord_attrib;
        GLint normal_attrib;

        GLint txMatrix_uniform;
        GLint texTxMatrix_uniform;
        GLint texUnit_uniform;
        GLint pointSize_uniform;
    public:
        RShader();
        RShader(const char* vertexSource, const char* fragSource);
        ~RShader();

        // RShader methods
        int init(const char* vertexSource, const char* fragSource);

        GLint getUniformLocation(const char* uniform) const;
        GLint getAttribLocation(const char* attrib)   const;

        GLint getVertexAttrib()   const;
        GLint getColorAttrib()    const;
        GLint getTexcoordAttrib() const;
        GLint getNormalAttrib()   const;

        GLint getTransformMatrixUniform() const;
        GLint getTextureTxMatrixUniform() const;
        GLint getTextureUnitUniform()     const;
        GLint getPointSizeUniform()       const;

        void attach()  const;
        void dettach() const;

        void destroy();
};
#endif