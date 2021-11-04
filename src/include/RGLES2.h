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

#ifndef _BV
#define _BV(x) ((1) << (x))
#endif

#ifndef DEG2RAD
#define DEG2RAD(x) ((x) * M_PI / 180.f);
#endif

#define DEFAULT_DRAW_BUFFER_SIZE_ELEMENTS 512
#define CIRCLE_STEPS 32

#define rmalloc(n)    malloc(n)
#define rrealloc(p,n) realloc(p,n)
#define rfree(p)      free(p)

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

        // Do not use for now! Enyx is NOT intended to be a 3D graphics engine (yet)
        static RMatrix4 translation(float tx, float ty, float tz);
        static RMatrix4 rotation(float angle);
        static RMatrix4 scaling(float sx, float sy);

        // Do not use for now! Enyx is NOT intended to be a 3D graphics engine (yet)
        static RMatrix4 scaling(float sx, float sy, float sz); 

        static RMatrix4 ortho(float left, float right, float bottom, float top, float znear, float zfar);

        // This is not going to be a 3D engine, right?
        static RMatrix4 frustum(float left, float right, float bottom, float top, float znear, float zfar);
};

// Renderer base structs
typedef struct {
    float x, y, z;
} __attribute__((packed)) vertex3_t;

typedef struct {
    float x, y, z;
} __attribute__((packed)) normal3_t;

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
// This should be a global struct accesible from the rendering pipelines
struct rperfstats_t {
    // Total number of drawcalls
    uint32_t drawcalls;
    // Total vertices drawn (only vertices, not color / texture) per frame operation
    uint32_t vertices_drawn;
    // Total context changes operations
    uint32_t context_changes;
    // Number of auxiliary buffers used in this frame
    uint32_t auxiliary_buffers_used;
    // Maximum buffer usage in elements!
    uint32_t buffer_max_elements_used;
    // Total bytes transfered via glAttibPointer / glTexImage2D operation
    uint32_t bytes_transfered;
    // Total time usage for the draw operation (newTime - lastTime)
    uint32_t time_ms;
    // ...
};

// Struct for bufferptr_t
struct rbufferptr_t {
    // Vertex pointer
    void* vtx_ptr;
    // Normal pointer
    void* nrm_ptr;
    // Color pointer
    void* clr_ptr;
    // Texcoord pointer
    void* txc_ptr;
};

// Enum for buffer header flags
enum rbufferheader_flags_t {
    FLAG_NONE     = 0,
    FLAG_TEMPORAL = _BV(0)
};
// Struct for buffer header
struct rbufferheader_t {
    // Buffer size in bytes (Without header)
    uint32_t buffer_size;
    // Buffer size in elements
    uint32_t buffer_max_elements;

    // Number of elements (glDrawArrays)
    uint32_t elements;
    // Current vertex, color and texcoord elements count (One vtx element = 3 floats (vertex3_t))
    uint32_t vtx_count;
    uint32_t clr_count;
    uint32_t txc_count;
    // Buffer offsets counting from base + RBUFFERHEADER_SIZE
    intptr_t vtx_offset;
    intptr_t clr_offset;
    intptr_t txc_offset;
    // flags? / textures? / parameters?
    uint32_t flags;
};

#define RBUFFERHEADER_SIZE sizeof(rbufferheader_t)

// Info for OpenGL ES 2.0 renderer
struct rgles2info_t {
    GLint MAX_FRAGMENT_UNIFORM_VECTORS;
    GLint MAX_VERTEX_UNIFORM_VECTORS;
    GLint MAX_VARYING_VECTORS;
    GLint MAX_VERTEX_ATTRIBS;
    GLint MAX_RENDERBUFFER_SIZE;
    GLint MAX_TEXTURE_IMAGE_UNITS;
    GLint MAX_COMBINED_TEXTURE_IMAGE_UNITS;
    GLint MAX_CUBE_MAP_TEXTURE_SIZE;
    GLint MAX_TEXTURE_SIZE;
    // Notify on 0. Vertex texture fetch NOT supported when 0.
    GLint MAX_VERTEX_TEXTURE_IMAGE_UNITS;
    GLint MAX_VIEWPORT_DIMS_WIDTH;
    GLint MAX_VIEWPORT_DIMS_HEIGHT;
};


// Shader class
class RShader {
    private:
        GLuint programId;

        GLint vertex_attrib;
        GLint color_attrib;
        GLint texcoord_attrib;

        GLint tmtrx_uniform;
        GLint tex0_uniform;
    public:
        RShader();
        ~RShader();
        RShader(const char* vertexSource, const char* fragSource);

        int init(const char* vertexSource, const char* fragSource);

        GLint getUniformLocation(const char* uniform);
        GLint getAttribLocation(const char* attrib);

        GLint getVertexAttrib()   const;
        GLint getColorAttrib()    const;
        GLint getTexcoordAttrib() const;

        GLint getTransformMatrixUniform() const;
        GLint getTextureUnitUniform()     const;

        // Attach, dettach (Enable / Disable attributes and glUseProgram)
        void attach()  const;
        void dettach() const;
};


class RPipeline {
    public:
        virtual ~RPipeline();
        // Enable rendering pipeline (Only on context change)
        virtual void enable()  = 0;
        // Disable rendering pipeline (Only on context change)
        virtual void disable() = 0;
        // Draw method (Receives pointer to auxiliary buffer)
        virtual void draw(void* buffer) = 0;
        // Set uniforms!
        virtual void setTransform(RMatrix4& matrix) = 0;
};


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
};

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

// ...

// class RGLES2 : public AGL {
class RGLES2 {
    private:
        // Internal variables and methods
        Window*       baseWindow;
        SDL_GLContext gContext;

        // Rendering buffer, DO NOT CONFUSE WITH AN OpenGL RenderBuffer
        void* drawBuffer;
        // Number of MAX elements in the draw buffer. Used via 
        uint32_t drawBufferSizeElements;
        int circle_steps;

        RPipeline* currentRPipeline;

        // Where's the Matrix?
        RMatrix4 tMatrix;
        // Pipelines
        RDotPipeline*      dotPipeline;
        RLinePipeline*     linePipeline;
        RTrianglePipeline* trianglePipeline;
        // RTexturePipeline*  texturePipeline;

        // Renderer info struct 
        rgles2info_t gles2_info;
        // Internal methods
        // Switch pipeline: Only if new pipeline is different to the new pipeline
        void setPipeline(RPipeline* pipeline);

        // Zero performance stats counter. Call every frame!
        // Or not
        void zeroPerfstats();

        // Generate draw buffers / resize draw buffer
        void* genDrawBuffers(void* drawBuffer, uint32_t drawBufferElements);

        // Request elements for drawing
        void* allocateElements(size_t elements, rbufferptr_t* rbufferptr);

        // Update drawing buffers before allocation
       void updateBuffer(void* buffer, size_t vtx, size_t nrm, size_t clr, size_t txc);

       // Update transform
       void updateTransform();
    public:
        RGLES2();
        ~RGLES2();

        /**
         * @brief Set the Window object. Must be done before calling start
         * 
         * @param window 
         */
        void setWindow(Window* window);

        /**
         * @brief Sets (or resizes) number of draw buffer elements
         * 
         * @param buffer_elements 
         */
        int setMaxBufferElements(uint32_t buffer_elements);

        /**
         * @brief Starts the RGLES2 renderer.
         * 
         * @return int Returns zero on sucess, other on error
         */
        int  init();

        /**
         * @brief Ends the RGLES2 renderer. Called automatically from the destructor
         * 
         * @return int Returns zero on sucess
         */
        int destroy();

        /**
         * @brief Submits and clears current buffers to OpenGL API. This function can be called automatically
         * 
         */
        void submit();

        /**
         * @brief Submits and clears current buffers to OpenGL API. This function can be called automatically
         * 
         * @param buffer drawBuffer to use
         */
        void submit(void* buffer);

        /**
         * @brief Ends a rendering operation and update swap-chain
         * 
         */
        void render();

        /**
         * @brief Clear the rendering buffers
         * 
         */
        void clearBuffers();


        // Viewport, scissor and coordinate transformations!

        /**
         * @brief Sets the renderer viewport
         * 
         * @param x Viewport X position
         * @param y Viewport Y position
         * @param w Viewport width
         * @param h Viewport height
         */
        void viewport(int x, int y, int w, int h);

        /**
         * @brief Resets the renderer viewport (window size)
         * 
         */
        void viewport();

        /**
         * @brief Enables and sets scissor testing
         * 
         * @param x Scissor rectangle X coordinate
         * @param y Scissor rectangle Y coordinate
         * @param w Scissor rectangle width
         * @param h Scissor rectangle height
         */
        void scissor(int x, int y, int w, int h);

        /**
         * @brief Disables scissor testing
         * 
         */
        void scissor();


        // Coordinate transformations

        /**
         * @brief Resets coordinate transformations
         * 
         */

        void origin();

        /**
         * @brief Translates to tx, ty
         * 
         * @param tx 
         * @param ty 
         */
        void translate(float tx, float ty);

        /**
         * @brief Translates to tx, ty
         * 
         * @param tx 
         * @param ty 
         */
        void translate(int tx, int ty);

        /**
         * @brief Rotates angle radians
         * 
         * @param angle 
         */
        void rotate(float angle);

        /**
         * @brief Scales by x,y
         * 
         * @param x 
         * @param y 
         */
        void scale(float x, float y);

        /**
         * @brief Scales by x,y
         * 
         * @param x 
         * @param y 
         */
        void scale(int x, int y);

        /**
         * @brief Get the Transformation Matrix object
         * 
         * @return RMatrix4 
         */
        RMatrix4 getTransformationMatrix() const;

        /**
         * @brief Set the Transfomation Matrix object
         * 
         * @param tmatrix 
         */
        void     setTransfomationMatrix(const RMatrix4& tmatrix);


        // RENDERING METHODS!
        void clearColor(color_t color);
        void clear();
        // void fillScreen(color_t color);

        /**
         * @brief Draws a pixel in coordinate x,y with color color
         * 
         * @param x 
         * @param y 
         * @param color 
         */
        void drawPixel(int x, int y, color_t color);

        /**
         * @brief Draws a line from x0,y0 to x1,y1
         * 
         * @param x0 
         * @param y0 
         * @param x1 
         * @param y1 
         * @param color 
         */
        void drawLine(int x0, int y0, int x1, int y1, color_t color);

        /**
         * @brief Draw a line from x0,y0 to x1,y1 with a color gradient
         * 
         * @param x0 
         * @param y0 
         * @param x1 
         * @param y1 
         * @param color1 
         * @param color2 
         */
        void drawLine(int x0, int y0, int x1, int y1, color_t color1, color_t color2);

        /**
         * @brief Draw a fast vertical line.
         * 
         * @param x0 
         * @param y0 
         * @param length 
         * @param color 
         * 
         * This method calls drawLine in RGLES2 renderer
         */
        void drawFastVLine(int x0, int y0, int length, color_t color);

        /**
         * @brief Draw a fast horizontal line.
         * 
         * @param x0 
         * @param y0 
         * @param length 
         * @param color 
         * 
         * This method calls drawLine in RGLES2 renderer
         */
        void drawFastHLine(int x0, int y0, int length, color_t color);

        /**
         * @brief Draws a rectagle
         * 
         * @param x 
         * @param y 
         * @param w 
         * @param h 
         * @param color 
         */
        void drawRect(int x, int y, int w, int h, color_t color);

        /**
         * @brief Draws a fill rectangle
         * 
         * @param x 
         * @param y 
         * @param w 
         * @param h 
         * @param color 
         */
        void drawFillRect(int x, int y, int w, int h, color_t color);

        /**
         * @brief Draws a circle
         * 
         * @param x 
         * @param y 
         * @param r 
         * @param color 
         */
        void drawCircle(int x, int y, int r, color_t color);

        /**
         * @brief Draws a filled circle
         * 
         * @param x 
         * @param y 
         * @param r 
         * @param color 
         */
        void drawFillCircle(int x, int y, int r, color_t color);

        /**
         * @brief Draws a filled circle with a color gradient
         * 
         * @param x 
         * @param y 
         * @param r 
         * @param color1 
         * @param color2 
         */
        void drawFillCircle(int x, int y, int r, color_t color1, color_t color2);

        /**
         * @brief Draws a triangle (lines)
         * 
         * @param x0 Vertex 1 coordinate
         * @param y0 Vertex 1 coordinate
         * @param x1 Vertex 2 coordinate
         * @param y1 Vertex 2 coordinate
         * @param x2 Vertex 3 coordinate
         * @param y2 Vertex 3 coordinate
         * @param color Triangle line color
         */
        void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);


        /**
         * @brief Draws a filled triangle
         * 
         * @param x0 Vertex 1 coordinate
         * @param y0 Vertex 1 coordinate
         * @param x1 Vertex 2 coordinate
         * @param y1 Vertex 2 coordinate
         * @param x2 Vertex 3 coordinate
         * @param y2 Vertex 3 coordinate
         * @param color Triangle fill color
         */
        void drawFillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);

        /**
         * @brief Draws OpenGL "Hello triangle" style filled triangle
         * 
         * @param x0 Vertex 1 coordinate
         * @param y0 Vertex 1 coordinate
         * @param x1 Vertex 2 coordinate
         * @param y1 Vertex 2 coordinate
         * @param x2 Vertex 3 coordinate
         * @param y2 Vertex 3 coordinate
         * @param color1 Color for vertex 1
         * @param color2 Color for vertex 2
         * @param color3 Color for vertex 3
         */
        void drawFillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color1, color_t color2, color_t color3);

};

#endif