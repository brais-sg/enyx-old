/**
 * @file RShader.cpp
 * @author Brais Solla González
 * @brief Enyx RGLES2 Shader implementation for OpenGL ES 2.0 (Only!)
 * @version 0.1
 * @date 2021-11-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "RGLES2/RGLES2.h"
#include "RGLES2/RShader.h"
#include "Debug.h"

#include <GLES2/gl2.h>

RShader::RShader(){
    this->programId = 0;

    this->vertex_attrib   = -1;
    this->color_attrib    = -1;
    this->texcoord_attrib = -1;
    this->normal_attrib   = -1;


    this->txMatrix_uniform    = -1;
    this->texTxMatrix_uniform = -1;
    this->texUnit_uniform     = -1;
    this->pointSize_uniform   = -1;
}

RShader::~RShader(){
    if(this->programId){
        // Debug::info("[%s:%d]: Deleting OpenGL program %d\n", __FILE__, __LINE__, this->programId);
        // glDeleteProgram(this->programId);
        // this->programId = 0;
        this->destroy();
    } else {
        Debug::info("[%s:%d]: Deleting non initialized OpenGL program, skipping...\n", __FILE__, __LINE__);
    }
}

RShader::RShader(const char* vertexSource, const char* fragSource){
    this->programId = 0;

    this->vertex_attrib   = -1;
    this->color_attrib    = -1;
    this->texcoord_attrib = -1;
    this->normal_attrib   = -1;


    this->txMatrix_uniform    = -1;
    this->texTxMatrix_uniform = -1;
    this->texUnit_uniform     = -1;
    this->pointSize_uniform   = -1;

    if(this->init(vertexSource, fragSource)){
        Debug::error("[%s:%d]: Shader program creation error in constructor!\n", __FILE__, __LINE__);
    }
}

int RShader::init(const char* vertexSource, const char* fragSource){
    GLuint vert, frag;

    vert = glCreateShader(GL_VERTEX_SHADER);
    frag = glCreateShader(GL_FRAGMENT_SHADER);

    if(vert == 0 || frag == 0){
        Debug::error("[%s:%d]: Cannot create vertex or fragment shader object!\n", __FILE__, __LINE__);
        return -1;
    }

    glShaderSource(vert, 1, &vertexSource, NULL);
    glShaderSource(frag, 1, &fragSource,   NULL);

    // Compile vertex shader
    glCompileShader(vert);

    GLint vertex_compiled;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &vertex_compiled);
    if(vertex_compiled != GL_TRUE){
        GLsizei log_length = 0;
        GLchar message[2048];
        glGetShaderInfoLog(vert, 2048, &log_length, message);

        Debug::error("[%s:%d]: Vertex shader compiler error!\n", __FILE__, __LINE__);
        Debug::error("%s\n", message);
        return -2;
    }

    // Compile fragment shader
    glCompileShader(frag);

    GLint fragment_compiled;
    glGetShaderiv(frag, GL_COMPILE_STATUS, &fragment_compiled);
    if(fragment_compiled != GL_TRUE){
        GLsizei log_length = 0;
        GLchar message[2048];
        glGetShaderInfoLog(frag, 2048, &log_length, message);
        Debug::error("[%s:%d]: Fragment shader compiler error!\n", __FILE__, __LINE__);
        Debug::error("%s\n", message);
        return -3;
    }

    // Prepare for linkage
    if(this->programId){
        Debug::warning("[%s:%d]: programId already exists (%d), removing and linking new shader...\n", __FILE__, __LINE__, (int) this->programId);
        glDeleteProgram(this->programId);
    }

    this->programId = glCreateProgram();
    if(this->programId == 0){
        Debug::error("[%s:%d]: Cannot create the program object!\n", __FILE__, __LINE__);
        return -4;
    }

    // Attrib location for program
    glBindAttribLocation(this->programId, 0, "a_vertex");
    glBindAttribLocation(this->programId, 1, "a_color");
    glBindAttribLocation(this->programId, 2, "a_vtxcoord");
    glBindAttribLocation(this->programId, 3, "a_normal");

    // Attach and link shader
    glAttachShader(this->programId, vert);
    glAttachShader(this->programId, frag);

    glLinkProgram(this->programId);

    // Linkage info
    GLint program_linked;
    glGetProgramiv(this->programId, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE){
        GLsizei log_length = 0;
        GLchar message[2048];
        glGetProgramInfoLog(this->programId, 2048, &log_length, message);
        Debug::error("[%s:%d]: Shader linkage error!\n", __FILE__, __LINE__);
        Debug::error("%s\n", message);
        return -5;
    }

    // Get location for vertex attribs / uniforms
    // Do not trust the driver with vertex attribs!
    this->vertex_attrib        = this->getAttribLocation("a_vertex");
    this->color_attrib         = this->getAttribLocation("a_color");
    this->texcoord_attrib      = this->getAttribLocation("a_vtxcoord");
    this->normal_attrib        = this->getAttribLocation("a_normal");

    this->txMatrix_uniform     = this->getUniformLocation("u_tmtrx");
    this->texUnit_uniform      = this->getUniformLocation("u_textureunit");
    this->texTxMatrix_uniform  = this->getUniformLocation("u_txtmtrx");
    this->pointSize_uniform    = this->getUniformLocation("u_pointsize");

    // Warnings for missing texmatrix!
    if(this->txMatrix_uniform == -1){
        Debug::warning("[%s:%d]: Shader %d is missing a transformation matrix uniform!\n", __FILE__, __LINE__, (int) this->programId);
    }

    // Free shaders
    glDeleteShader(vert);
    glDeleteShader(frag);

    return 0;
}


GLint RShader::getUniformLocation(const char* uniform) const {
    return glGetUniformLocation(this->programId, uniform);
}

GLint RShader::getAttribLocation(const char* attrib) const {
    return glGetAttribLocation(this->programId, attrib);
}

GLint RShader::getVertexAttrib() const {
    return this->vertex_attrib;
}

GLint RShader::getColorAttrib() const {
    return this->color_attrib;
}

GLint RShader::getTexcoordAttrib() const {
    return this->texcoord_attrib;
}

GLint RShader::getNormalAttrib() const {
    return this->normal_attrib;
}

GLint RShader::getTransformMatrixUniform() const {
    return this->txMatrix_uniform;
}

GLint RShader::getTextureTxMatrixUniform() const {
    return this->texTxMatrix_uniform;
}

GLint RShader::getTextureUnitUniform() const {
    return this->texUnit_uniform;
}

GLint RShader::getPointSizeUniform() const {
    return this->pointSize_uniform;
}

void RShader::attach() const {
    glUseProgram(this->programId);

    if(this->vertex_attrib   != -1)   glEnableVertexAttribArray(this->vertex_attrib);
    if(this->color_attrib    != -1)   glEnableVertexAttribArray(this->color_attrib);
    if(this->texcoord_attrib != -1)   glEnableVertexAttribArray(this->texcoord_attrib);
    if(this->normal_attrib   != -1)   glEnableVertexAttribArray(this->normal_attrib);
}

void RShader::dettach() const {
    if(this->vertex_attrib   != -1) glDisableVertexAttribArray(this->vertex_attrib);
    if(this->color_attrib    != -1) glDisableVertexAttribArray(this->color_attrib);
    if(this->texcoord_attrib != -1) glDisableVertexAttribArray(this->texcoord_attrib);
    if(this->normal_attrib   != -1) glDisableVertexAttribArray(this->normal_attrib);
}

void RShader::destroy(){
    if(this->programId){
        Debug::info("[%s:%d]: Deleting program %d...\n", __FILE__, __LINE__, this->programId);
        glDeleteProgram(this->programId);
        this->programId = 0;
    } else {
        Debug::info("[%s:%d]: This shader is not in use!\n",__FILE__, __LINE__);
    }
}

