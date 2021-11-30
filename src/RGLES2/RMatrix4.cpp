/**
 * @file RMatrix4.cpp
 * @author Brais Solla González
 * @brief RMatrix4 implementation
 * @version 0.1
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "RGLES2/RMatrix4.h"

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