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
#define DEFAULT_CIRCLE_STEPS 32

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

enum renderer_info_t {
    RENDERER_INFO_NONE     = 0,
    RENDERER_INFO_BACKEND  = 1,
    RENDERER_INFO_VENDOR   = 2,
    RENDERER_INFO_VERSION  = 3,
    RENDERER_INFO_GPU      = 4
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

        Matrix4(const Matrix4& other);

        Matrix4 operator+(const Matrix4& other) const;
        Matrix4 operator*(const Matrix4& other) const;


        Matrix4& operator=(const Matrix4& other);
        
        void loadIdentity();
        float* getArray();

        static Matrix4 translation(float tx, float ty);
        static Matrix4 translation(float tx, float ty, float tz); // Do not use for now! Enyx is NOT intended to be a 3D graphics engine (yet)
        static Matrix4 rotation(float angle);
        static Matrix4 scaling(float sx, float sy);
        static Matrix4 scaling(float sx, float sy, float sz); // Do not use for now! Enyx is NOT intended to be a 3D graphics engine (yet)

        static Matrix4 ortho(float left, float right, float bottom, float top, float znear, float zfar);
};

class Renderer_SDL2_GLES2 : public AGL {
    private:
        Window*       window;
        SDL_GLContext gContext;

        GL_Shader* currentShader;

        float* v3pos_array;
        float* v4col_array;
        float* v2txc_array;

        int v3pos_count;
        int v4col_count;
        int v2txc_count;

        int max_elements;
        int current_elements;

        drawing_state_t d_state;

        // Circle steps
        int circle_steps;

        // Shaders
        GL_Shader basicShader;
        GL_Shader textureShader;

        // Transformation Matrix
        Matrix4 transform;

        // Internal helper methods (Exposed for advanced usage)
        void setTransform();
        void setDrawingState(drawing_state_t drawing_mode);
        void bufferElements(int space);

        void appendVtx(float vx, float vy, float vz);
        void appendCol(float r, float g, float b, float a);
        void appendTxc(float u, float v);

        void setShader(GL_Shader* shader);
    public:
        Renderer_SDL2_GLES2();
        Renderer_SDL2_GLES2(Window* window);
        ~Renderer_SDL2_GLES2();

        // Renderer info
        const char* getRendererInfo(renderer_info_t renderer);

        // init and setWindow
        void setWindow(Window* window);
        int  init();

        // Batch size
        void setBatchSize(uint32_t batch_size);
        int  getBatchSize()        const;
        int  getCurrentBatchSize() const;

        // Implementation of AGL API

        // Viewport
        void viewport(int x, int y, int w, int h);
        void viewport();

        // Scissor
        void scissor(int x, int y, int w, int h);
        void scissor();

        // Coordinate transformations
        void origin();
        void translate(float tx, float ty);
        void translate(int tx, int ty);
        void rotate(float angle);
        void scale(float sx, float sy);
        void scale(int sx, int sy);

        // Renderer specific
        Matrix4 getTransformationMatrix();
        void    setTransformationMatrix(Matrix4 matrix);

        // DRAWING METHODS
        void drawPixel(int x, int y, color_t color);
        void drawLine(int x0, int y0, int x1, int y1, color_t color);
        void drawLine(int x0, int y0, int x1, int y1, color_t color1, color_t color2);

        void drawFastVLine(int x0, int y0, int length, color_t color);
        void drawFastHLine(int x0, int y0, int length, color_t color);

        void drawRect(int x, int y, int w, int h, color_t color);
        void drawFillRect(int x, int y, int w, int h, color_t color);

        void drawCircle(int x, int y, int r, color_t color);
        void drawFillCircle(int x, int y, int r, color_t color);
        void drawFillCircle(int x, int y, int r, color_t color1, color_t color2);

        void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
        void drawFillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);

        // Classic OpenGL "Hello triangle" with colors
        void drawFillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color1, color_t color2, color_t color3);

        int getWidth()      const;
        int getHeight()     const;
        int getPixelDepth() const;

        // Clear operations
        void clearColor(color_t color);
        void clear();
        void fillScreen(color_t color);

        // Submit updates to OpenGL
        void submit();
        void render();


};

#endif