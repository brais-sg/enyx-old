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

#ifndef sq
#define sq(x) (((x)*(x)))
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

