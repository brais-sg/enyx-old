/**
 * @file RGLES2.h
 * @author Brais Solla González
 * @brief RGLES2 / Renderer for OpenGL ES 2.0 API
 * @version 0.1
 * @date 2021-10-14
 * 
 * @copyright Copyright (c) 2021
 * 
 * Enyx OpenGL ES 2.0 Rendering backend (rewrite from Renderer_SDL2_GLES2.h)
 * This file uses libSDL2!
 */

#ifndef _RGLES2_INCLUDED
#define _RGLES2_INCLUDED
#include "AGL.h"
#include "Platform_SDL2.h"
#include <GLES2/gl2.h>

// Renderer base classes definition
class RVector2i {
    public:
        int x, y;

        RVector2i();
        RVector2i(int x, int y);
        RVector2i(const RVector2i& other);

        RVector2i operator+(const RVector2i& other) const;
        RVector2i operator-(const RVector2i& other) const;
        RVector2i operator*(float scalar) const;

        RVector2i& operator+=(const RVector2i& other);
        RVector2i& operator-=(const RVector2i& other);
        RVector2i& operator=(const RVector2i& other);
        RVector2i& operator*=(float scalar);
        RVector2i& normalize();

        RVector2i normalized() const;

        float length()   const;
        float sqLength() const;
        float angle()    const;

        static float dot(const RVector2i& v1, const RVector2i& v2);
        static RVector2i lerp(const RVector2i& v1, const RVector2i& v2, float n);

};

class RVector2 {
    public:
        float x, y;

        RVector2();
        RVector2(float x, float y);
        RVector2(const RVector2& other);
        RVector2(const RVector2i& other);

        RVector2 operator+(const RVector2& other) const;
        RVector2 operator-(const RVector2& other) const;
        RVector2 operator*(float scalar) const;

        RVector2& operator+=(const RVector2& other);
        RVector2& operator-=(const RVector2& other);
        RVector2& operator=(const RVector2& other);
        RVector2& operator*=(float scalar);
        RVector2& normalize();

        RVector2 normalized() const;

        float length()   const;
        float sqLength() const;
        float angle()    const;

        static float dot(const RVector2& v1, const RVector2& v2);
        static RVector2 lerp(const RVector2& v1, const RVector2& v2, float n);
};

class RMatrix4 {
    public:
        float e[16];

        RMatrix4();
        RMatrix4(const RMatrix4& other);

        RMatrix4 operator+(const RMatrix4& other) const;
        RMatrix4 operator-(const RMatrix4& other) const;
        RMatrix4 operator*(const RMatrix4& other) const;
        RMatrix4 operator*(float scalar) const;

        RMatrix4& operator+=(const RMatrix4& other);
        RMatrix4& operator-=(const RMatrix4& other);
        RMatrix4& operator*=(const RMatrix4& other);
        RMatrix4& operator=(const RMatrix4& other);
        RMatrix4& operator*=(float scalar);

        // Methods

        float* getArray();
        void   loadIdentity();

        static RMatrix4 translation(float tx, float ty);
        static RMatrix4 translation(float tx, float ty, float tz); // Do not use for now! Enyx is NOT intended to be a 3D graphics engine (yet)
        static RMatrix4 rotation(float angle);
        static RMatrix4 scaling(float sx, float sy);
        static RMatrix4 scaling(float sx, float sy, float sz); // Do not use for now! Enyx is NOT intended to be a 3D graphics engine (yet)

        static RMatrix4 ortho(float left, float right, float bottom, float top, float znear, float zfar);
};

// Renderer base structs
typedef struct {
    float x, y, z;
} __attribute__((packed)) vertex3_t;

typedef struct {
    float r, g, b, a;
} __attribute__((packed)) color4_t;

typedef struct {
    float s, t;
} __attribute__((packed)) texcrd2_t;

typedef struct {
    float s, t, u;
} __attribute__((packed)) texcrd3_t;

// Performance counter struct
struct rperfstats_t {
    // ...
};

// Shader class
class RShader {
    private:
        GLint programId;

        GLint color_attrib;
        GLint texcoord_attrib;

        GLint tmtrx_uniform;
        GLint textureunit_uniform;
    public:
        RShader();
        ~RShader();
        RShader(const char* vertexSource, const char* fragSource);

        int init(const char* vertexSource, const char* fragSource);

        GLint getUniformLocation(const char* uniform);
        GLint getAttribLocation(const char* attribute);

        GLint getVertexAttrib()   const;
        GLint getColorAttrib()    const;
        GLint getTexcoordAttrib() const;

        GLint getTransformMatrixUniform() const;
        GLint getTextureUnitUniform()     const;

        // Attach, dettach (Enable / Disable attributes and glUseProgram)
        void attach()  const;
        void dettach() const;
};

#endif