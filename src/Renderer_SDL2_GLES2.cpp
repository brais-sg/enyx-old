/**
 * @file Renderer_SDL2_GLES2.cpp
 * @author Brais Solla González
 * @brief Enyx GL Renderer for SDL2 and OpenGL ES 2.0 API
 * @version 0.1
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "AGL.h"
#include "Renderer_SDL2_GLES2.h"

#include "shaders/glsl_es/basic.h"
#include "shaders/glsl_es/texture.h"

GL_Shader::GL_Shader(){
    program_id = 0; 

    vertex_shader_id    = 0;
    fragment_shader_id  = 0;

    vertex_attrib       = -1;
    color_attrib        = -1;
    texcoord_attrib     = -1;

    tmtrx_uniform       = -1;
    textureunit_uniform = -1;
}

GL_Shader::~GL_Shader(){
    if(program_id){
        fprintf(stderr, "[%s:%d]: ~GL_Shader(): Deleting program id = %d\n", __FILE__, __LINE__, (int) program_id);
        glDeleteProgram(program_id);
    }

    if(vertex_shader_id)   glDeleteShader(vertex_shader_id);
    if(fragment_shader_id) glDeleteShader(fragment_shader_id);
}


void GL_Shader::shaderSource(shader_type_t type, const char* source){
    if(type == VERTEX_SHADER){
        vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_id, 1, &source, NULL);
    } else if(type == FRAGMENT_SHADER){
        fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_id, 1, &source, NULL);
    } else {
        fprintf(stderr,"[%s:%d]: WARNING: shaderSource(): Shader format not supported for OpenGL ES 2.0 context!\n", __FILE__, __LINE__);
        return;
    }
}

int GL_Shader::compileShader(){
    if(!vertex_shader_id){
        fprintf(stderr, "[%s:%d]: ERROR: compileShader(): No vertex shader!\n", __FILE__, __LINE__);
        return -1;
    }

    if(!fragment_shader_id){
        fprintf(stderr, "[%s:%d]: ERROR: compileShader(): No fragment shader!\n", __FILE__, __LINE__);
        return -2;
    }

    glCompileShader(vertex_shader_id);
    GLint vertex_compiled;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &vertex_compiled);
    if(vertex_compiled != GL_TRUE){
        GLsizei log_length = 0;
        GLchar message[2048];
        glGetShaderInfoLog(vertex_shader_id, 2048, &log_length, message);
        fprintf(stderr, "[%s:%d]: ERROR: compileShader(): Vertex shader compiler error!\n", __FILE__, __LINE__);
        fprintf(stderr, "%s\n", message);
        return -3;
    }

    glCompileShader(fragment_shader_id);
    GLint fragment_compiled;
    if(fragment_compiled != GL_TRUE){
        GLsizei log_length = 0;
        GLchar message[2048];
        glGetShaderInfoLog(fragment_shader_id, 2048, &log_length, message);
        fprintf(stderr, "[%s:%d]: ERROR: compileShader(): Fragment shader compiler error!\n", __FILE__, __LINE__);
        fprintf(stderr, "%s\n", message);
        return -4;
    }


    return 0;
}

int GL_Shader::linkShader(){
    if(program_id) glDeleteProgram(program_id);

    program_id = glCreateProgram();

    if(!vertex_shader_id){
        fprintf(stderr, "[%s:%d]: ERROR: linkShader(): No vertex shader!\n", __FILE__, __LINE__);
        return -1;
    }

    if(!fragment_shader_id){
        fprintf(stderr, "[%s:%d]: ERROR: linkShader(): No fragment shader!\n", __FILE__, __LINE__);
        return -2;
    }


    // Attrib location for program
    glBindAttribLocation(program_id, 0, "a_vertex");
    glBindAttribLocation(program_id, 1, "a_color");
    glBindAttribLocation(program_id, 2, "a_vtxcoord");

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);

    GLint program_linked;
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[2048];
        glGetProgramInfoLog(program_id, 2048, &log_length, message);
        fprintf(stderr, "[%s:%d]: ERROR: linkShader(): shader linkage error!\n", __FILE__, __LINE__);
        fprintf(stderr, "%s\n", message);
        return -3;
    }

    // Location for all attributes / uniforms
    vertex_attrib        = this->getAttribLocation("a_vertex");
    color_attrib         = this->getAttribLocation("a_color");
    texcoord_attrib      = this->getAttribLocation("a_vtxcoord");

    tmtrx_uniform        = this->getUniformLocation("u_tmtrx");
    textureunit_uniform  = this->getUniformLocation("u_textureunit");


    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    vertex_shader_id   = 0;
    fragment_shader_id = 0;

    
    return 0;
}


GLint GL_Shader::getUniformLocation(const char* uniform){
    return glGetUniformLocation(program_id, uniform);
}

GLint GL_Shader::getAttribLocation(const char* attribute){
    return glGetAttribLocation(program_id, attribute);
}


GLint GL_Shader::getVertexAttrib() const {
    return vertex_attrib;
}

GLint GL_Shader::getColorAttrib() const {
    return color_attrib;
}

GLint GL_Shader::getTexcoordAttrib() const {
    return texcoord_attrib;
}

GLint GL_Shader::getTransformMatrixUniform() const {
    return tmtrx_uniform;
}

GLint GL_Shader::getTextureUnitUniform() const {
    return textureunit_uniform;
}

void GL_Shader::attach(){
    glUseProgram(this->program_id);

    if(vertex_attrib)   glEnableVertexAttribArray(vertex_attrib);
    if(color_attrib)    glEnableVertexAttribArray(color_attrib);
    if(texcoord_attrib) glEnableVertexAttribArray(texcoord_attrib);
}

void GL_Shader::dettach(){
    if(vertex_attrib)   glDisableVertexAttribArray(vertex_attrib);
    if(color_attrib)    glDisableVertexAttribArray(color_attrib);
    if(texcoord_attrib) glDisableVertexAttribArray(texcoord_attrib);

    // glUseProgram(0);
}

// Matrix4 implementation here

/*  
 *  0  4  8  12
 *  1  5  9  13
 *  2  6  10 14
 *  3  7  11 15
 */

Matrix4::Matrix4(){
    e[0]  = 0.0f;
    e[1]  = 0.0f;
    e[2]  = 0.0f;
    e[3]  = 0.0f;
    e[4]  = 0.0f;
    e[5]  = 0.0f;
    e[6]  = 0.0f;
    e[7]  = 0.0f;
    e[8]  = 0.0f;
    e[9]  = 0.0f;
    e[10] = 0.0f;
    e[11] = 0.0f;
    e[12] = 0.0f;
    e[13] = 0.0f;
    e[14] = 0.0f;
    e[15] = 0.0f;
}

Matrix4 Matrix4::operator+(const Matrix4& other) const {
    Matrix4 _ret = Matrix4();

    _ret.e[0]  = this->e[0]  + other.e[0];
    _ret.e[1]  = this->e[1]  + other.e[1];
    _ret.e[2]  = this->e[2]  + other.e[2];
    _ret.e[3]  = this->e[3]  + other.e[3];
    _ret.e[4]  = this->e[4]  + other.e[4];
    _ret.e[5]  = this->e[5]  + other.e[5];
    _ret.e[6]  = this->e[6]  + other.e[6];
    _ret.e[7]  = this->e[7]  + other.e[7];
    _ret.e[8]  = this->e[8]  + other.e[8];
    _ret.e[9]  = this->e[9]  + other.e[9];
    _ret.e[10] = this->e[10] + other.e[10];
    _ret.e[11] = this->e[11] + other.e[11];
    _ret.e[12] = this->e[12] + other.e[12];
    _ret.e[13] = this->e[13] + other.e[13];
    _ret.e[14] = this->e[14] + other.e[14];
    _ret.e[15] = this->e[15] + other.e[15];

    return _ret;
}

/*  
 *  0  4  8  12
 *  1  5  9  13
 *  2  6  10 14
 *  3  7  11 15
 */

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 _ret = Matrix4();

    _ret.e[0]  = this->e[0] * other.e[0] + this->e[4] * other.e[1] + this->e[8]  * other.e[2] + this->e[12] * other.e[3];
    _ret.e[1]  = this->e[1] * other.e[0] + this->e[5] * other.e[1] + this->e[9]  * other.e[2] + this->e[13] * other.e[3];
    _ret.e[2]  = this->e[2] * other.e[0] + this->e[6] * other.e[1] + this->e[10] * other.e[2] + this->e[14] * other.e[3];
    _ret.e[3]  = this->e[3] * other.e[0] + this->e[7] * other.e[1] + this->e[11] * other.e[2] + this->e[15] * other.e[3];

    _ret.e[4]  = this->e[0] * other.e[4] + this->e[4] * other.e[5] + this->e[8]  * other.e[6] + this->e[12] * other.e[7];
    _ret.e[5]  = this->e[1] * other.e[4] + this->e[5] * other.e[5] + this->e[9]  * other.e[6] + this->e[13] * other.e[7];
    _ret.e[6]  = this->e[2] * other.e[4] + this->e[6] * other.e[5] + this->e[10] * other.e[6] + this->e[14] * other.e[7];
    _ret.e[7]  = this->e[3] * other.e[4] + this->e[7] * other.e[5] + this->e[11] * other.e[6] + this->e[15] * other.e[7];

    _ret.e[8]  = this->e[0] * other.e[8] + this->e[4] * other.e[9] + this->e[8]  * other.e[10] + this->e[12] * other.e[11];
    _ret.e[9]  = this->e[1] * other.e[8] + this->e[5] * other.e[9] + this->e[9]  * other.e[10] + this->e[13] * other.e[11];
    _ret.e[10] = this->e[2] * other.e[8] + this->e[6] * other.e[9] + this->e[10] * other.e[10] + this->e[14] * other.e[11];
    _ret.e[11] = this->e[3] * other.e[8] + this->e[7] * other.e[9] + this->e[11] * other.e[10] + this->e[15] * other.e[11];

    _ret.e[12] = this->e[0] * other.e[12] + this->e[4] * other.e[13] + this->e[8]  * other.e[14] + this->e[12] * other.e[15];
    _ret.e[13] = this->e[1] * other.e[12] + this->e[5] * other.e[13] + this->e[9]  * other.e[14] + this->e[13] * other.e[15];
    _ret.e[14] = this->e[2] * other.e[12] + this->e[6] * other.e[13] + this->e[10] * other.e[14] + this->e[14] * other.e[15];
    _ret.e[15] = this->e[3] * other.e[12] + this->e[7] * other.e[13] + this->e[11] * other.e[14] + this->e[15] * other.e[15];

    return _ret;
}

void Matrix4::loadIdentity(){
    e[0]  = 1.0f;
    e[1]  = 0.0f;
    e[2]  = 0.0f;
    e[3]  = 0.0f;
    e[4]  = 0.0f;
    e[5]  = 1.0f;
    e[6]  = 0.0f;
    e[7]  = 0.0f;
    e[8]  = 0.0f;
    e[9]  = 0.0f;
    e[10] = 1.0f;
    e[11] = 0.0f;
    e[12] = 0.0f;
    e[13] = 0.0f;
    e[14] = 0.0f;
    e[15] = 1.0f;
}

float* Matrix4::getArray(){
    return this->e;
}

/*  
 *  0  4  8  12
 *  1  5  9  13
 *  2  6  10 14
 *  3  7  11 15
 */


Matrix4 Matrix4::translation(float tx, float ty){
    Matrix4 R = Matrix4();
    R.loadIdentity();

    R.e[12] = tx;
    R.e[13] = ty;
    R.e[14] = 1.f;

    return R;
}

Matrix4 Matrix4::translation(float tx, float ty, float tz){
    Matrix4 R = Matrix4();
    R.loadIdentity();

    R.e[12] = tx;
    R.e[13] = ty;
    R.e[14] = tz;

    return R;
}

Matrix4 Matrix4::rotating(float angle){
    Matrix4 R = Matrix4();
    R.loadIdentity();

    float rcos = cos(angle);
    float rsin = sin(angle);

    R.e[0] = rcos;
    R.e[1] = rsin;
    R.e[4] = -rsin;
    R.e[5] = rcos;

    return R;
}

Matrix4 Matrix4::scaling(float sx, float sy){
    Matrix4 R = Matrix4();
    R.loadIdentity();

    R.e[0] = sx;
    R.e[5] = sy;

    return R;
}

Matrix4 Matrix4::scaling(float sx, float sy, float sz){
    Matrix4 R = Matrix4();
    R.loadIdentity();

    R.e[0]  = sx;
    R.e[5]  = sy;
    R.e[10] = sz;

    return R;
}

/*  
 *  0  4  8  12
 *  1  5  9  13
 *  2  6  10 14
 *  3  7  11 15
 */

Matrix4 Matrix4::ortho(float left, float right, float bottom, float top, float znear, float zfar){
    Matrix4 R = Matrix4();
    // R.loadIdentity(); // Not needed! Optimized

    R.e[0]  = 2.0f / (right - left);
    R.e[5]  = 2.0f / (top - bottom);
    R.e[10] = 2.0f / (zfar - znear);

    // Translate vector
    R.e[12] = -(right + left) / (right - left);
    R.e[13] = -(top + bottom) / (top - bottom);
    R.e[14] = -(zfar + znear) / (zfar - znear);
    R.e[15] = 1.f;

    return R;
}


// Renderer implementation starts here
Renderer_SDL2_GLES2::Renderer_SDL2_GLES2(){
    window   = NULL;
    gContext = NULL;

    currentShader = NULL;

    v3pos_array = NULL;
    v4col_array = NULL;
    v2txc_array = NULL;

    v3pos_count = 0;
    v4col_count = 0;
    v2txc_count = 0;

    max_elements     = 0;
    current_elements = 0;

    d_state = DRAWING_STATE_NONE;
}


Renderer_SDL2_GLES2::Renderer_SDL2_GLES2(Window* window){
    this->window = window;
    gContext     = NULL;

    currentShader = NULL;

    v3pos_array = NULL;
    v4col_array = NULL;
    v2txc_array = NULL;

    v3pos_count = 0;
    v4col_count = 0;
    v2txc_count = 0;

    max_elements     = 0;
    current_elements = 0;

    d_state = DRAWING_STATE_NONE;
}


Renderer_SDL2_GLES2::~Renderer_SDL2_GLES2(){
    if(v3pos_array) free(v3pos_array);
    if(v4col_array) free(v4col_array);
    if(v2txc_array) free(v2txc_array);

    max_elements     = 0;
    current_elements = 0;

    SDL_GL_DeleteContext(gContext);
    d_state = DRAWING_STATE_NONE;
}


void Renderer_SDL2_GLES2::setWindow(Window* window){
    if(this->window){
        fprintf(stderr, "[%s:%d]: ERROR: setWindow(): this->window already set!\n", __FILE__, __LINE__);
        return;
    }
    this->window = window;
}

int Renderer_SDL2_GLES2::init(){
    // Init the SDL2 GLES2 rendering engine!
    if(this->window == NULL){
        fprintf(stderr, "[%s:%d]: ERROR: source window not set!\n", __FILE__, __LINE__);
        return -1;
    }

    fprintf(stderr,"[%s:%d]: INFO: Starting SDL2 OpenGL ES 2.0 Enyx renderer...\n", __FILE__, __LINE__);
    window->GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    window->GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    window->GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    if(window->GL_SetSwapInterval(-1) != 0){
        fprintf(stderr, "[%s:%d]: WARNING: init(): Adaptative sync failed! Trying VSync = on...\n", __FILE__, __LINE__);
        if(window->GL_SetSwapInterval(1) != 0){
            fprintf(stderr, "[%s:%d]: WARNING: init(): Cannot enable Vsync!\n", __FILE__, __LINE__);
            window->GL_SetSwapInterval(0);
        }
    }
    
    if(window->GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0){
        fprintf(stderr, "[%s:%d]: WARNING: init(): Cannot enable double buffer!\n", __FILE__, __LINE__);
    }

    if(window->GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) != 0){
        fprintf(stderr, "[%s:%d]: WARNING: init(): Cannot set 24 bits depth buffer!\n", __FILE__, __LINE__);
        fprintf(stderr, "[%s:%d]: WARNING: init(): Setting 16 bits depth buffer instead\n", __FILE__, __LINE__);
        window->GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    }
    
    this->gContext = window->GL_CreateContext();
    if(this->gContext == NULL){
        fprintf(stderr, "[%s:%d]: ERROR: init(): Cannot create OpenGL ES 2.0 context!\n", __FILE__, __LINE__);
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }

    fprintf(stderr,"[%s:%d]: INFO: OpenGL ES 2.0 compatible context created!\n", __FILE__, __LINE__);
    fprintf(stderr,"[%s:%d]: INFO: GL_VENDOR:                   %s\n", __FILE__, __LINE__, glGetString(GL_VENDOR));
    fprintf(stderr,"[%s:%d]: INFO: GL_RENDERER:                 %s\n", __FILE__, __LINE__, glGetString(GL_RENDERER));
    fprintf(stderr,"[%s:%d]: INFO: GL_VERSION:                  %s\n", __FILE__, __LINE__, glGetString(GL_VERSION));
    fprintf(stderr,"[%s:%d]: INFO: GL_SHADING_LANGUAGE_VERSION: %s\n", __FILE__, __LINE__, glGetString(GL_SHADING_LANGUAGE_VERSION));


    window->GL_MakeCurrent(this->gContext);

    // Prepare for shader compilation
    basicShader    = GL_Shader();
    textureShader  = GL_Shader();

    basicShader.shaderSource(VERTEX_SHADER,   basic_vert);
    basicShader.shaderSource(FRAGMENT_SHADER, basic_frag);

    if(basicShader.compileShader()){
        fprintf(stderr, "[%s:%d]: ERROR: init(): Cannot compile basicShader\n", __FILE__, __LINE__);
        return -2;
    }

    if(basicShader.linkShader()){
        fprintf(stderr, "[%s:%d]: ERROR: init(): Cannot link basicShader\n", __FILE__, __LINE__);
        return -2;
    }

    textureShader.shaderSource(VERTEX_SHADER,   texture_vert);
    textureShader.shaderSource(FRAGMENT_SHADER, texture_frag);

    if(textureShader.compileShader()){
        fprintf(stderr, "[%s:%d]: ERROR: init(): Cannot compile textureShader\n", __FILE__, __LINE__);
        return -2;
    }

    if(textureShader.linkShader()){
        fprintf(stderr, "[%s:%d]: ERROR: init(): Cannot link textureShader\n", __FILE__, __LINE__);
        return -2;
    }

    // Generate arrays
    max_elements = DEFAULT_RENDERER_BATCH_SIZE;
    this->current_elements = 0;
    this->v3pos_count      = 0;
    this->v4col_count      = 0;
    this->v2txc_count      = 0;

    v3pos_array = (float*) malloc(max_elements * 3 * sizeof(float));
    v4col_array = (float*) malloc(max_elements * 4 * sizeof(float));
    v2txc_array = (float*) malloc(max_elements * 2 * sizeof(float));

    memset(v3pos_array, 0, max_elements * 3 * sizeof(float));
    memset(v4col_array, 0, max_elements * 4 * sizeof(float));
    memset(v2txc_array, 0, max_elements * 2 * sizeof(float));

    
    currentShader = NULL;
    d_state = DRAWING_STATE_READY;

    return 0;
}


void Renderer_SDL2_GLES2::setBatchSize(uint32_t batch_size){
    this->max_elements = batch_size;
    this->current_elements = 0;
    this->v3pos_count      = 0;
    this->v4col_count      = 0;
    this->v2txc_count      = 0;

    if(v3pos_array) v3pos_array = (float*) realloc(v3pos_array, max_elements * 3 * sizeof(float));
    if(v4col_array) v4col_array = (float*) realloc(v4col_array, max_elements * 4 * sizeof(float));
    if(v2txc_array) v2txc_array = (float*) realloc(v2txc_array, max_elements * 2 * sizeof(float)); 
}

int Renderer_SDL2_GLES2::getBatchSize() const {
    return max_elements;
}

int Renderer_SDL2_GLES2::getCurrentBatchSize() const {
    return current_elements;
}


// Renderer implementation starts here

void Renderer_SDL2_GLES2::viewport(int x, int y, int w, int h){
    glViewport(x, y, w, h);
}

void Renderer_SDL2_GLES2::viewport(){
    int wx = window->getWidth();
    int wy = window->getHeight();

    glViewport(0, 0, wx, wy);
}

void Renderer_SDL2_GLES2::scissor(int x, int y, int w, int h){
    glScissor(x, y, w, h);
    glEnable(GL_SCISSOR_TEST);
}

void Renderer_SDL2_GLES2::scissor(){
    glDisable(GL_SCISSOR_TEST);
}

