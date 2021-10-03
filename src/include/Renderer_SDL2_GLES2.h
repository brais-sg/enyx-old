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

class GL_Shader {
    private:
        GLuint program_id;

        GLuint vertex_shader_id;
        GLuint fragment_shader_id;
    public:
        GL_Shader();
        ~GL_Shader();

        void shaderSource(shader_type_t type, const char* source);
        void compileShader();
        void linkShader();

        // Enable / disable attributes
        void attachShader();
        void dettachShader();
};

class Renderer_SDL2_GLES2 : public AGL {
    private:
        Window*       window;
        SDL_GLContext gContext;

        GL_Shader* currentShader;

        float* v3pos_array;
        float* v4col_array;
        float* v2txc_array;

        uint32_t max_elements;
        uint32_t current_elements;
    public:
        Renderer_SDL2_GLES2();
        Renderer_SDL2_GLES2(Window* window);
        ~Renderer_SDL2_GLES2();













};

#endif