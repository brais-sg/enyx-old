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
