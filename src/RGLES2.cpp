/**
 * @file RGLES2.cpp
 * @author Brais Solla González
 * @brief Enyx OpenGL ES 2.0 renderer implementation
 * @version 0.1
 * @date 2021-10-14
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
#include "RGLES2.h"
#include "Debug.h"

// Internal shaders
#include "shaders/glsl_es/basic.h"
#include "shaders/glsl_es/texture.h"

#ifndef sq
#define sq(x) (((x)*(x)))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

// Base class

// RVector2i implementation
RVector2i::RVector2i(){
    this->x = 0;
    this->y = 0;
}

RVector2i::RVector2i(int x, int y){
    this->x = x;
    this->y = y;
}

RVector2i::RVector2i(const RVector2i& other){
    this->x = other.x;
    this->y = other.y;
}

RVector2i RVector2i::operator+(const RVector2i& other) const {
    RVector2i ret = RVector2i(this->x + other.x, this->y + other.y);
    return ret;
}

RVector2i RVector2i::operator-(const RVector2i& other) const {
    RVector2i ret = RVector2i(this->x - other.x, this->y - other.y);
    return ret;
}

RVector2i RVector2i::operator*(float scalar) const {
    RVector2i ret = RVector2i(this->x * scalar, this->y * scalar);
    return ret;
}

RVector2i& RVector2i::operator+=(const RVector2i& other){
    this->x += other.x;
    this->y += other.y;

    return *this;
}

RVector2i& RVector2i::operator-=(const RVector2i& other){
    this->x -= other.x;
    this->y -= other.y;
    
    return *this;
}

RVector2i& RVector2i::operator=(const RVector2i& other){
    this->x = other.x;
    this->y = other.y;

    return *this;
}

RVector2i& RVector2i::operator*=(float scalar){
    this->x *= scalar;
    this->y *= scalar;
    
    return *this;
}

RVector2i& RVector2i::normalize(){
    float inv_sq = 1.f / this->length();

    this->x *= inv_sq;
    this->y *= inv_sq;

    return *this;
}

RVector2i RVector2i::normalized() const {
    float inv_sq = 1.f / this->length();

    RVector2i ret = RVector2i(this->x * inv_sq, this->y * inv_sq);
    return ret;
}

float RVector2i::length() const {
    return sqrt(sq(this->x) + sq(this->y));
}

float RVector2i::sqLength() const {
    return (float) (sq(this->x) + sq(this->y));
}

float RVector2i::angle() const {
    return (float) atan2((float) this->y, (float) this->x);
}

float RVector2i::dot(const RVector2i& v1, const RVector2i& v2){
    return (v1.x * v2.x + v1.y * v2.y);
}

RVector2i RVector2i::lerp(const RVector2i& v1, const RVector2i& v2, float n){
    return v1 + (v2 - v1) * n;
}


// RVector2 implementation
RVector2::RVector2(){
    this->x = 0.f;
    this->y = 0.f;
}

RVector2::RVector2(float x, float y){
    this->x = x;
    this->y = y;
}

RVector2::RVector2(const RVector2& other){
    this->x = other.x;
    this->y = other.y;
}

RVector2::RVector2(const RVector2i& other){
    this->x = (float) other.x;
    this->y = (float) other.y;
}

RVector2 RVector2::operator+(const RVector2& other) const {
    RVector2 ret = RVector2(this->x + other.x, this->y + other.y);
    return ret;
}

RVector2 RVector2::operator-(const RVector2& other) const {
    RVector2 ret = RVector2(this->x - other.x, this->y - other.y);
    return ret;
}

RVector2 RVector2::operator*(float scalar) const {
    RVector2 ret = RVector2(this->x * scalar, this->y * scalar);
    return ret;
}

RVector2& RVector2::operator+=(const RVector2& other){
    this->x += other.x;
    this->y += other.y;

    return *this;
}

RVector2& RVector2::operator-=(const RVector2& other){
    this->x -= other.x;
    this->y -= other.y;
    
    return *this;
}

RVector2& RVector2::operator=(const RVector2& other){
    this->x = other.x;
    this->y = other.y;

    return *this;
}

RVector2& RVector2::operator*=(float scalar){
    this->x *= scalar;
    this->y *= scalar;
    
    return *this;
}

RVector2& RVector2::normalize(){
    float inv_sq = 1.f / this->length();

    this->x *= inv_sq;
    this->y *= inv_sq;

    return *this;
}

RVector2 RVector2::normalized() const {
    float inv_sq = 1.f / this->length();

    RVector2 ret = RVector2(this->x * inv_sq, this->y * inv_sq);
    return ret;
}

float RVector2::length() const {
    return sqrt(sq(this->x) + sq(this->y));
}

float RVector2::sqLength() const {
    return (float) (sq(this->x) + sq(this->y));
}

float RVector2::angle() const {
    return (float) atan2((float) this->y, (float) this->x);
}

float RVector2::dot(const RVector2& v1, const RVector2& v2){
    return (v1.x * v2.x + v1.y * v2.y);
}

RVector2 RVector2::lerp(const RVector2& v1, const RVector2& v2, float n){
    return v1 + (v2 - v1) * n;
}

// RMatrix4 implementation
RMatrix4::RMatrix4(){
    this->e[0]  = 0.f;
    this->e[1]  = 0.f;
    this->e[2]  = 0.f;
    this->e[3]  = 0.f;
    this->e[4]  = 0.f;
    this->e[5]  = 0.f;
    this->e[6]  = 0.f;
    this->e[7]  = 0.f;
    this->e[8]  = 0.f;
    this->e[9]  = 0.f;
    this->e[10] = 0.f;
    this->e[11] = 0.f;
    this->e[12] = 0.f;
    this->e[13] = 0.f;
    this->e[14] = 0.f;
    this->e[15] = 0.f;
}

RMatrix4::RMatrix4(const RMatrix4& other){
    this->e[0]  = other.e[0];
    this->e[1]  = other.e[1];
    this->e[2]  = other.e[2];
    this->e[3]  = other.e[3];
    this->e[4]  = other.e[4];
    this->e[5]  = other.e[5];
    this->e[6]  = other.e[6];
    this->e[7]  = other.e[7];
    this->e[8]  = other.e[8];
    this->e[9]  = other.e[9];
    this->e[10] = other.e[10];
    this->e[11] = other.e[11];
    this->e[12] = other.e[12];
    this->e[13] = other.e[13];
    this->e[14] = other.e[14];
    this->e[15] = other.e[15];
}

RMatrix4 RMatrix4::operator+(const RMatrix4& other) const {
    RMatrix4 ret = RMatrix4();

    ret.e[0]  = this->e[0]  + other.e[0];
    ret.e[1]  = this->e[1]  + other.e[1];
    ret.e[2]  = this->e[2]  + other.e[2];
    ret.e[3]  = this->e[3]  + other.e[3];
    ret.e[4]  = this->e[4]  + other.e[4];
    ret.e[5]  = this->e[5]  + other.e[5];
    ret.e[6]  = this->e[6]  + other.e[6];
    ret.e[7]  = this->e[7]  + other.e[7];
    ret.e[8]  = this->e[8]  + other.e[8];
    ret.e[9]  = this->e[9]  + other.e[9];
    ret.e[10] = this->e[10] + other.e[10];
    ret.e[11] = this->e[11] + other.e[11];
    ret.e[12] = this->e[12] + other.e[12];
    ret.e[13] = this->e[13] + other.e[13];
    ret.e[14] = this->e[14] + other.e[14];
    ret.e[15] = this->e[15] + other.e[15];

    return ret;
}

RMatrix4 RMatrix4::operator-(const RMatrix4& other) const {
    RMatrix4 ret = RMatrix4();

    ret.e[0]  = this->e[0]  - other.e[0];
    ret.e[1]  = this->e[1]  - other.e[1];
    ret.e[2]  = this->e[2]  - other.e[2];
    ret.e[3]  = this->e[3]  - other.e[3];
    ret.e[4]  = this->e[4]  - other.e[4];
    ret.e[5]  = this->e[5]  - other.e[5];
    ret.e[6]  = this->e[6]  - other.e[6];
    ret.e[7]  = this->e[7]  - other.e[7];
    ret.e[8]  = this->e[8]  - other.e[8];
    ret.e[9]  = this->e[9]  - other.e[9];
    ret.e[10] = this->e[10] - other.e[10];
    ret.e[11] = this->e[11] - other.e[11];
    ret.e[12] = this->e[12] - other.e[12];
    ret.e[13] = this->e[13] - other.e[13];
    ret.e[14] = this->e[14] - other.e[14];
    ret.e[15] = this->e[15] - other.e[15];

    return ret;
}

RMatrix4 RMatrix4::operator*(const RMatrix4& other) const {
    RMatrix4 ret = RMatrix4();

    ret.e[0]  = this->e[0] * other.e[0] + this->e[4] * other.e[1] + this->e[8]  * other.e[2] + this->e[12] * other.e[3];
    ret.e[1]  = this->e[1] * other.e[0] + this->e[5] * other.e[1] + this->e[9]  * other.e[2] + this->e[13] * other.e[3];
    ret.e[2]  = this->e[2] * other.e[0] + this->e[6] * other.e[1] + this->e[10] * other.e[2] + this->e[14] * other.e[3];
    ret.e[3]  = this->e[3] * other.e[0] + this->e[7] * other.e[1] + this->e[11] * other.e[2] + this->e[15] * other.e[3];

    ret.e[4]  = this->e[0] * other.e[4] + this->e[4] * other.e[5] + this->e[8]  * other.e[6] + this->e[12] * other.e[7];
    ret.e[5]  = this->e[1] * other.e[4] + this->e[5] * other.e[5] + this->e[9]  * other.e[6] + this->e[13] * other.e[7];
    ret.e[6]  = this->e[2] * other.e[4] + this->e[6] * other.e[5] + this->e[10] * other.e[6] + this->e[14] * other.e[7];
    ret.e[7]  = this->e[3] * other.e[4] + this->e[7] * other.e[5] + this->e[11] * other.e[6] + this->e[15] * other.e[7];

    ret.e[8]  = this->e[0] * other.e[8] + this->e[4] * other.e[9] + this->e[8]  * other.e[10] + this->e[12] * other.e[11];
    ret.e[9]  = this->e[1] * other.e[8] + this->e[5] * other.e[9] + this->e[9]  * other.e[10] + this->e[13] * other.e[11];
    ret.e[10] = this->e[2] * other.e[8] + this->e[6] * other.e[9] + this->e[10] * other.e[10] + this->e[14] * other.e[11];
    ret.e[11] = this->e[3] * other.e[8] + this->e[7] * other.e[9] + this->e[11] * other.e[10] + this->e[15] * other.e[11];

    ret.e[12] = this->e[0] * other.e[12] + this->e[4] * other.e[13] + this->e[8]  * other.e[14] + this->e[12] * other.e[15];
    ret.e[13] = this->e[1] * other.e[12] + this->e[5] * other.e[13] + this->e[9]  * other.e[14] + this->e[13] * other.e[15];
    ret.e[14] = this->e[2] * other.e[12] + this->e[6] * other.e[13] + this->e[10] * other.e[14] + this->e[14] * other.e[15];
    ret.e[15] = this->e[3] * other.e[12] + this->e[7] * other.e[13] + this->e[11] * other.e[14] + this->e[15] * other.e[15];

    return ret;
}

RMatrix4 RMatrix4::operator*(float scalar) const {
    RMatrix4 ret = RMatrix4();

    ret.e[0]  = this->e[0]  * scalar;
    ret.e[1]  = this->e[1]  * scalar;
    ret.e[2]  = this->e[2]  * scalar;
    ret.e[3]  = this->e[3]  * scalar;
    ret.e[4]  = this->e[4]  * scalar;
    ret.e[5]  = this->e[5]  * scalar;
    ret.e[6]  = this->e[6]  * scalar;
    ret.e[7]  = this->e[7]  * scalar;
    ret.e[8]  = this->e[8]  * scalar;
    ret.e[9]  = this->e[9]  * scalar;
    ret.e[10] = this->e[10] * scalar;
    ret.e[11] = this->e[11] * scalar;
    ret.e[12] = this->e[12] * scalar;
    ret.e[13] = this->e[13] * scalar;
    ret.e[14] = this->e[14] * scalar;
    ret.e[15] = this->e[15] * scalar;

    return ret;
}

RMatrix4& RMatrix4::operator+=(const RMatrix4& other){
    this->e[0]  += other.e[0];
    this->e[1]  += other.e[1];
    this->e[2]  += other.e[2];
    this->e[3]  += other.e[3];
    this->e[4]  += other.e[4];
    this->e[5]  += other.e[5];
    this->e[6]  += other.e[6];
    this->e[7]  += other.e[7];
    this->e[8]  += other.e[8];
    this->e[9]  += other.e[9];
    this->e[10] += other.e[10];
    this->e[11] += other.e[11];
    this->e[12] += other.e[12];
    this->e[13] += other.e[13];
    this->e[14] += other.e[14];
    this->e[15] += other.e[15];

    return *this;
}

RMatrix4& RMatrix4::operator-=(const RMatrix4& other){
    this->e[0]  -= other.e[0];
    this->e[1]  -= other.e[1];
    this->e[2]  -= other.e[2];
    this->e[3]  -= other.e[3];
    this->e[4]  -= other.e[4];
    this->e[5]  -= other.e[5];
    this->e[6]  -= other.e[6];
    this->e[7]  -= other.e[7];
    this->e[8]  -= other.e[8];
    this->e[9]  -= other.e[9];
    this->e[10] -= other.e[10];
    this->e[11] -= other.e[11];
    this->e[12] -= other.e[12];
    this->e[13] -= other.e[13];
    this->e[14] -= other.e[14];
    this->e[15] -= other.e[15];

    return *this;
}

RMatrix4& RMatrix4::operator*=(const RMatrix4& other){
    RMatrix4 temp = RMatrix4();

    temp.e[0]  = this->e[0] * other.e[0] + this->e[4] * other.e[1] + this->e[8]  * other.e[2] + this->e[12] * other.e[3];
    temp.e[1]  = this->e[1] * other.e[0] + this->e[5] * other.e[1] + this->e[9]  * other.e[2] + this->e[13] * other.e[3];
    temp.e[2]  = this->e[2] * other.e[0] + this->e[6] * other.e[1] + this->e[10] * other.e[2] + this->e[14] * other.e[3];
    temp.e[3]  = this->e[3] * other.e[0] + this->e[7] * other.e[1] + this->e[11] * other.e[2] + this->e[15] * other.e[3];

    temp.e[4]  = this->e[0] * other.e[4] + this->e[4] * other.e[5] + this->e[8]  * other.e[6] + this->e[12] * other.e[7];
    temp.e[5]  = this->e[1] * other.e[4] + this->e[5] * other.e[5] + this->e[9]  * other.e[6] + this->e[13] * other.e[7];
    temp.e[6]  = this->e[2] * other.e[4] + this->e[6] * other.e[5] + this->e[10] * other.e[6] + this->e[14] * other.e[7];
    temp.e[7]  = this->e[3] * other.e[4] + this->e[7] * other.e[5] + this->e[11] * other.e[6] + this->e[15] * other.e[7];

    temp.e[8]  = this->e[0] * other.e[8] + this->e[4] * other.e[9] + this->e[8]  * other.e[10] + this->e[12] * other.e[11];
    temp.e[9]  = this->e[1] * other.e[8] + this->e[5] * other.e[9] + this->e[9]  * other.e[10] + this->e[13] * other.e[11];
    temp.e[10] = this->e[2] * other.e[8] + this->e[6] * other.e[9] + this->e[10] * other.e[10] + this->e[14] * other.e[11];
    temp.e[11] = this->e[3] * other.e[8] + this->e[7] * other.e[9] + this->e[11] * other.e[10] + this->e[15] * other.e[11];

    temp.e[12] = this->e[0] * other.e[12] + this->e[4] * other.e[13] + this->e[8]  * other.e[14] + this->e[12] * other.e[15];
    temp.e[13] = this->e[1] * other.e[12] + this->e[5] * other.e[13] + this->e[9]  * other.e[14] + this->e[13] * other.e[15];
    temp.e[14] = this->e[2] * other.e[12] + this->e[6] * other.e[13] + this->e[10] * other.e[14] + this->e[14] * other.e[15];
    temp.e[15] = this->e[3] * other.e[12] + this->e[7] * other.e[13] + this->e[11] * other.e[14] + this->e[15] * other.e[15];

    *this = temp;
    return *this;
}

RMatrix4& RMatrix4::operator=(const RMatrix4& other){
    this->e[0]  = other.e[0];
    this->e[1]  = other.e[1];
    this->e[2]  = other.e[2];
    this->e[3]  = other.e[3];
    this->e[4]  = other.e[4];
    this->e[5]  = other.e[5];
    this->e[6]  = other.e[6];
    this->e[7]  = other.e[7];
    this->e[8]  = other.e[8];
    this->e[9]  = other.e[9];
    this->e[10] = other.e[10];
    this->e[11] = other.e[11];
    this->e[12] = other.e[12];
    this->e[13] = other.e[13];
    this->e[14] = other.e[14];
    this->e[15] = other.e[15];

    return *this;
}

RMatrix4& RMatrix4::operator*=(float scalar){
    this->e[0]  *= scalar;
    this->e[1]  *= scalar;
    this->e[2]  *= scalar;
    this->e[3]  *= scalar;
    this->e[4]  *= scalar;
    this->e[5]  *= scalar;
    this->e[6]  *= scalar;
    this->e[7]  *= scalar;
    this->e[8]  *= scalar;
    this->e[9]  *= scalar;
    this->e[10] *= scalar;
    this->e[11] *= scalar;
    this->e[12] *= scalar;
    this->e[13] *= scalar;
    this->e[14] *= scalar;
    this->e[15] *= scalar;

    return *this;
}

float* RMatrix4::getArray(){
    return this->e;
}

void RMatrix4::loadIdentity(){
    this->e[0]  = 1.f;
    this->e[1]  = 0.f;
    this->e[2]  = 0.f;
    this->e[3]  = 0.f;
    this->e[4]  = 0.f;
    this->e[5]  = 1.f;
    this->e[6]  = 0.f;
    this->e[7]  = 0.f;
    this->e[8]  = 0.f;
    this->e[9]  = 0.f;
    this->e[10] = 1.f;
    this->e[11] = 0.f;
    this->e[12] = 0.f;
    this->e[13] = 0.f;
    this->e[14] = 0.f;
    this->e[15] = 1.f;
}

RMatrix4 RMatrix4::translation(float tx, float ty){
    RMatrix4 ret = RMatrix4();
    ret.loadIdentity();

    ret.e[12] = tx;
    ret.e[13] = ty;
    ret.e[14] = 0.f;

    return ret;
}


RMatrix4 RMatrix4::translation(float tx, float ty, float tz){
    RMatrix4 ret = RMatrix4();
    ret.loadIdentity();

    ret.e[12] = tx;
    ret.e[13] = ty;
    ret.e[14] = tz;

    return ret;
}

RMatrix4 RMatrix4::rotation(float angle){
    RMatrix4 ret = RMatrix4();
    ret.loadIdentity();

    float rcos = cos(angle);
    float rsin = sin(angle);

    ret.e[0] = rcos;
    ret.e[1] = rsin;
    ret.e[4] = -rsin;
    ret.e[5] = rcos;

    return ret;
}

RMatrix4 RMatrix4::scaling(float sx, float sy){
    RMatrix4 ret = RMatrix4();
    ret.loadIdentity();

    ret.e[0]  = sx;
    ret.e[5]  = sy;
    ret.e[10] = 1.f;

    return ret;
}

RMatrix4 RMatrix4::scaling(float sx, float sy, float sz){
    RMatrix4 ret = RMatrix4();
    ret.loadIdentity();

    ret.e[0]  = sx;
    ret.e[5]  = sy;
    ret.e[10] = sz;

    return ret;
}

RMatrix4 RMatrix4::ortho(float left, float right, float bottom, float top, float znear, float zfar){
    RMatrix4 ret = RMatrix4();
    // ret.loadIdentity(); // Not needed! Optimized

    ret.e[0]  = 2.f / (right - left);
    ret.e[5]  = 2.f / (top - bottom);
    ret.e[10] = 2.f / (zfar - znear);

    // Translate vector
    ret.e[12] = -(right + left) / (right - left);
    ret.e[13] = -(top + bottom) / (top - bottom);
    ret.e[14] = -(zfar + znear) / (zfar - znear);
    ret.e[15] = 1.f;

    return ret;
}

RMatrix4 RMatrix4::frustum(float left, float right, float bottom, float top, float znear, float zfar){
    RMatrix4 ret = RMatrix4();
    // ret.loadIdentity(); // Not needed! Optimized

    ret.e[0]  = (2.f * znear) / (right - left);
    ret.e[5]  = (2.f * znear) / (top - bottom);

    ret.e[8]  = (right + left)  / (right - left);
    ret.e[9]  = (top + bottom)  / (top - bottom);
    ret.e[10] = -(zfar + znear) / (zfar - znear);
    ret.e[11] = -1.f;

    ret.e[14] = -(2.f * zfar * znear) / (zfar - znear);
    
    return ret;
}

// STEP 2: Implement base structs: In the header
// STEP 3: Implement shader class

RShader::RShader(){
    this->programId = 0;

    this->vertex_attrib   = -1;
    this->color_attrib    = -1;
    this->texcoord_attrib = -1;

    this->tmtrx_uniform       = -1;
    this->tex0_uniform        = -1;
}

RShader::~RShader(){
    if(this->programId){
        Debug::info("[%s:%d]: Deleting OpenGL program %d\n", __FILE__, __LINE__, (int) this->programId);
        glDeleteProgram(this->programId);
    } else {
        Debug::info("[%s:%d]: Deleting non initialized OpenGL program\n", __FILE__, __LINE__);
    }

    this->programId = 0;
}

RShader::RShader(const char* vertexSource, const char* fragSource){
    this->programId = 0;

    this->vertex_attrib   = -1;
    this->color_attrib    = -1;
    this->texcoord_attrib = -1;

    this->tmtrx_uniform       = -1;
    this->tex0_uniform        = -1;

    if(this->init(vertexSource, fragSource)){
        Debug::error("[%s:%d]: Shader program creation error in RShader constructor!\n", __FILE__, __LINE__);
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

    this->tmtrx_uniform        = this->getUniformLocation("u_tmtrx");
    this->tex0_uniform         = this->getUniformLocation("u_textureunit");

    // Warnings for missing texmatrix!
    if(this->tmtrx_uniform == -1){
        Debug::warning("[%s:%d]: Shader %d is missing a transformation matrix uniform!\n", __FILE__, __LINE__, (int) this->programId);
    }

    // Free shaders
    glDeleteShader(vert);
    glDeleteShader(frag);

    return 0;
}

GLint RShader::getUniformLocation(const char* uniform){
    return glGetUniformLocation(this->programId, uniform);
}

GLint RShader::getAttribLocation(const char* attrib){
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

GLint RShader::getTransformMatrixUniform() const {
    return this->tmtrx_uniform;
}

GLint RShader::getTextureUnitUniform() const {
    return this->tex0_uniform;
}

void RShader::attach() const {
    glUseProgram(this->programId);

    if(this->vertex_attrib != -1)   glEnableVertexAttribArray(this->vertex_attrib);
    if(this->color_attrib  != -1)   glEnableVertexAttribArray(this->color_attrib);
    if(this->texcoord_attrib != -1) glEnableVertexAttribArray(this->texcoord_attrib);
}

void RShader::dettach() const {
    if(this->vertex_attrib != -1)   glDisableVertexAttribArray(this->vertex_attrib);
    if(this->color_attrib  != -1)   glDisableVertexAttribArray(this->color_attrib);
    if(this->texcoord_attrib != -1) glDisableVertexAttribArray(this->texcoord_attrib);
}

// Just before pipelines, global rendering stats struct
static rperfstats_t perfstats;

static void zeroBufferElements(void* buffer){
    rbufferheader_t* header = (rbufferheader_t*) buffer;

    header->elements = 0;

    header->vtx_count = 0;
    header->clr_count = 0;
    header->txc_count = 0;
}

// IMPLEMENTING PIPELINES!
// Dot pipeline
RDotPipeline::RDotPipeline(){
    // Compile internal shader
    this->internalShader = RShader(basic_vert, basic_frag);
}

void RDotPipeline::enable(){
    this->internalShader.attach();
    // Disable some OpenGL states. We do NOT need blending in point rendering pipeline
    glDisable(GL_BLEND); 
    // Track number of context changes
    perfstats.context_changes++;
}

void RDotPipeline::disable(){
    this->internalShader.dettach();
}

void RDotPipeline::setTransform(RMatrix4& matrix){
    glUniformMatrix4fv(this->internalShader.getTransformMatrixUniform(), 1, GL_FALSE, matrix.getArray());
}

void RDotPipeline::draw(void* buffer){
    // Shader is currently attached via enable(). Only set the pointers and drawArrays
    // Also, update perfstats
    rbufferheader_t* header = (rbufferheader_t*) buffer;
    intptr_t buffer_base    = (intptr_t) header + RBUFFERHEADER_SIZE;

    void* vtxaddr = (void*) (buffer_base + header->vtx_offset);
    void* clraddr = (void*) (buffer_base + header->clr_offset);

    uint32_t element_count = header->elements;

    // Set OpenGL ES attrib pointers
    glVertexAttribPointer(this->internalShader.getVertexAttrib(), 3, GL_FLOAT, GL_FALSE, 0, vtxaddr);
    glVertexAttribPointer(this->internalShader.getColorAttrib(),  4, GL_FLOAT, GL_FALSE, 0, clraddr);

    // Draw arrays!
    glDrawArrays(GL_POINTS, 0, element_count);

    // Update performance counter struct and reset current elements in header
    perfstats.drawcalls++;
    perfstats.buffer_max_elements_used = max(perfstats.buffer_max_elements_used, element_count);
    perfstats.bytes_transfered        += (element_count * 3 * sizeof(float)) + (element_count * 4 * sizeof(float));
    perfstats.vertices_drawn          += element_count;
    // Zero elements in buffer and positions
    zeroBufferElements(buffer);
}

// Line pipeline
RLinePipeline::RLinePipeline(){
    this->internalShader = RShader(basic_vert, basic_frag);
}

void RLinePipeline::enable(){
    this->internalShader.attach();
    // Disable some OpenGL states. We do NOT need blending in line rendering pipeline
    glDisable(GL_BLEND); 
    // Track number of context changes
    perfstats.context_changes++;
}

void RLinePipeline::disable(){
    this->internalShader.dettach();
}

void RLinePipeline::setTransform(RMatrix4& matrix){
    glUniformMatrix4fv(this->internalShader.getTransformMatrixUniform(), 1, GL_FALSE, matrix.getArray());
}

void RLinePipeline::draw(void* buffer){
    rbufferheader_t* header = (rbufferheader_t*) buffer;
    intptr_t buffer_base    = (intptr_t) header + RBUFFERHEADER_SIZE;

    void* vtxaddr = (void*) (buffer_base + header->vtx_offset);
    void* clraddr = (void*) (buffer_base + header->clr_offset);

    uint32_t element_count = header->elements;

    // Set OpenGL ES attrib pointers
    glVertexAttribPointer(this->internalShader.getVertexAttrib(), 3, GL_FLOAT, GL_FALSE, 0, vtxaddr);
    glVertexAttribPointer(this->internalShader.getColorAttrib(),  4, GL_FLOAT, GL_FALSE, 0, clraddr);

    // Draw arrays!
    glDrawArrays(GL_LINES, 0, element_count);

    // Update performance counter struct and reset current elements in header
    perfstats.drawcalls++;
    perfstats.buffer_max_elements_used = max(perfstats.buffer_max_elements_used, element_count);
    perfstats.bytes_transfered        += (element_count * 3 * sizeof(float)) + (element_count * 4 * sizeof(float));
    perfstats.vertices_drawn          += element_count;
    // Zero elements in buffer and positions
    zeroBufferElements(buffer);
}

// Triangle (Fill) pipeline
RTrianglePipeline::RTrianglePipeline(){
    this->internalShader = RShader(basic_vert, basic_frag);
}

void RTrianglePipeline::enable(){
    this->internalShader.attach();

    // Enable blending in triangle pipeline
    glEnable(GL_BLEND);

    perfstats.context_changes++;
}

void RTrianglePipeline::disable(){
    this->internalShader.dettach();
}

void RTrianglePipeline::setTransform(RMatrix4& matrix){
    glUniformMatrix4fv(this->internalShader.getTransformMatrixUniform(), 1, GL_FALSE, matrix.getArray());
}

void RTrianglePipeline::draw(void* buffer){
    rbufferheader_t* header = (rbufferheader_t*) buffer;
    intptr_t buffer_base    = (intptr_t) header + RBUFFERHEADER_SIZE;

    void* vtxaddr = (void*) (buffer_base + header->vtx_offset);
    void* clraddr = (void*) (buffer_base + header->clr_offset);

    uint32_t element_count = header->elements;

    // Set OpenGL ES attrib pointers
    glVertexAttribPointer(this->internalShader.getVertexAttrib(), 3, GL_FLOAT, GL_FALSE, 0, vtxaddr);
    glVertexAttribPointer(this->internalShader.getColorAttrib(),  4, GL_FLOAT, GL_FALSE, 0, clraddr);

    // Draw arrays!
    glDrawArrays(GL_TRIANGLES, 0, element_count);

    // Update performance counter struct and reset current elements in header
    perfstats.drawcalls++;
    perfstats.buffer_max_elements_used = max(perfstats.buffer_max_elements_used, element_count);
    perfstats.bytes_transfered        += (element_count * 3 * sizeof(float)) + (element_count * 4 * sizeof(float));
    perfstats.vertices_drawn          += element_count;
    // Zero elements in buffer and positions
    zeroBufferElements(buffer);
}

// TODO. Set texture uniforms on texture pipeline enable!
