/**
 * @file Renderer_SDL2_GLES2.h
 * @author Brais Solla González
 * @brief Enyx GL Renderer for SDL2 and OpenGL ES 2.0 API
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef _RENDERER_SDL2_GLES2_INCLUDED
#define _RENDERER_SDL2_GLES2_INCLUDED

#include <GLES2/gl2.h>
#include "Platform_SDL2.h"
#include "AGL.h"

#define DEFAULT_RENDERER_BATCH_SIZE 512

enum shader_type_t {
    UNKNOWN_SHADER         = 0,
    VERTEX_SHADER          = 1,
    FRAGMENT_SHADER        = 2,
    GEOMETRY_SHADER        = 3,
    COMPUTE_SHADER         = 4,
    TESS_CONTROL_SHADER    = 5,
    TESS_EVALUATION_SHADER = 6,

    // ARB_vertex_program
    VERTEX_PROGRAM         = 98,
    // ARB_fragment_program
    FRAGMENT_PROGRAM       = 99
};

enum drawing_state_t {
    DRAWING_STATE_NONE   = 0,
    DRAWING_STATE_READY  = 1,

    DRAWING_STATE_POINTS             = 2,
    DRAWING_STATE_LINES              = 3,
    DRAWING_STATE_TRIANGLES          = 4,
    DRAWING_STATE_TEXTURED_TRIANGLES = 5
};


/*
    ATTRIB LOCATION (glBindAttribLocation)
     -> vertex position:  0
     -> color position:   1
     -> texture position: 2

*/

class GL_Shader {
    private:
        GLuint program_id;

        GLuint vertex_shader_id;
        GLuint fragment_shader_id;

        GLint vertex_attrib;
        GLint color_attrib;
        GLint texcoord_attrib;

        GLint tmtrx_uniform;
        GLint textureunit_uniform;
    public:
        GL_Shader();
        ~GL_Shader();

        void shaderSource(shader_type_t type, const char* source);
        int compileShader();
        int linkShader();

        GLint getUniformLocation(const char* uniform);
        GLint getAttribLocation(const char* attribute);

        GLint getVertexAttrib()   const;
        GLint getColorAttrib()    const;
        GLint getTexcoordAttrib() const;

        GLint getTransformMatrixUniform() const;
        GLint getTextureUnitUniform()     const;

        // Enable / disable attributes
        void attach();
        void dettach();
};

class Matrix4 {
    public:
        float e[16];  
        Matrix4();

        Matrix4 operator+(const Matrix4& other) const;
        Matrix4 operator*(const Matrix4& other) const;
        
        void loadIdentity();
        float* getArray();

        Matrix4 translation(float tx, float ty);
        Matrix4 translation(float tx, float ty, float tz); // Do not use for now! Enyx is NOT intended to be a 3D graphics engine (yet)
        Matrix4 rotating(float angle);
        Matrix4 scaling(float sx, float sy);
        Matrix4 scaling(float sx, float sy, float sz); // Do not use for now! Enyx is NOT intended to be a 3D graphics engine (yet)

        Matrix4 ortho(float left, float right, float bottom, float top, float znear, float zfar);
};

class Renderer_SDL2_GLES2 : public AGL {
    private:
        Window*       window;
        SDL_GLContext gContext;

        GL_Shader* currentShader;

        float* v3pos_array;
        float* v4col_array;
        float* v2txc_array;

        uint32_t v3pos_count;
        uint32_t v4col_count;
        uint32_t v2txc_count;

        uint32_t max_elements;
        uint32_t current_elements;

        drawing_state_t d_state;

        // Shaders
        GL_Shader basicShader;
        GL_Shader textureShader;
    public:
        Renderer_SDL2_GLES2();
        Renderer_SDL2_GLES2(Window* window);
        ~Renderer_SDL2_GLES2();

        // init and setWindow
        void setWindow(Window* window);
        int  init();

        // Batch size
        void setBatchSize(uint32_t batch_size);
        int  getBatchSize()        const;
        int  getCurrentBatchSize() const;

        // Implementation of AGL API
        void viewport(int x, int y, int w, int h);
        void viewport();

        void scissor(int x, int y, int w, int h);
        void scissor();














};

#endif