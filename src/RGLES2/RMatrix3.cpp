/**
 * @file RMatrix3.cpp
 * @author Brais Solla González
 * @brief RMatrix3 implementation
 * @version 0.1
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "RGLES2/RMatrix3.h"

RMatrix3::RMatrix3(){
    this->e[0] = 0.f;
    this->e[1] = 0.f;
    this->e[2] = 0.f;
    this->e[3] = 0.f;
    this->e[4] = 0.f;
    this->e[5] = 0.f;
    this->e[6] = 0.f;
    this->e[7] = 0.f;
    this->e[8] = 0.f;
}

RMatrix3::RMatrix3(const RMatrix3& other){
    this->e[0] = other.e[0];
    this->e[1] = other.e[1];
    this->e[2] = other.e[2];
    this->e[3] = other.e[3];
    this->e[4] = other.e[4];
    this->e[5] = other.e[5];
    this->e[6] = other.e[6];
    this->e[7] = other.e[7];
    this->e[8] = other.e[8];
}

RMatrix3 RMatrix3::operator+(const RMatrix3& other) const {
    RMatrix3 ret = RMatrix3();

    ret.e[0] = this->e[0] + other.e[0];
    ret.e[1] = this->e[1] + other.e[1];
    ret.e[2] = this->e[2] + other.e[2];
    ret.e[3] = this->e[3] + other.e[3];
    ret.e[4] = this->e[4] + other.e[4];
    ret.e[5] = this->e[5] + other.e[5];
    ret.e[6] = this->e[6] + other.e[6];
    ret.e[7] = this->e[7] + other.e[7];
    ret.e[8] = this->e[8] + other.e[8];

    return ret;
}

RMatrix3 RMatrix3::operator-(const RMatrix3& other) const {
    RMatrix3 ret = RMatrix3();

    ret.e[0] = this->e[0] - other.e[0];
    ret.e[1] = this->e[1] - other.e[1];
    ret.e[2] = this->e[2] - other.e[2];
    ret.e[3] = this->e[3] - other.e[3];
    ret.e[4] = this->e[4] - other.e[4];
    ret.e[5] = this->e[5] - other.e[5];
    ret.e[6] = this->e[6] - other.e[6];
    ret.e[7] = this->e[7] - other.e[7];
    ret.e[8] = this->e[8] - other.e[8];

    return ret;
}

RMatrix3 RMatrix3::operator*(const RMatrix3& other) const {
    RMatrix3 ret = RMatrix3();

    ret.e[0] = this->e[0] * other.e[0] + this->e[3] * other.e[1] + this->e[6] * other.e[2];
    ret.e[1] = this->e[1] * other.e[0] + this->e[4] * other.e[1] + this->e[7] * other.e[2];
    ret.e[2] = this->e[2] * other.e[0] + this->e[5] * other.e[1] + this->e[8] * other.e[2];

    ret.e[3] = this->e[0] * other.e[3] + this->e[3] * other.e[4] + this->e[6] * other.e[5];
    ret.e[4] = this->e[1] * other.e[3] + this->e[4] * other.e[4] + this->e[7] * other.e[5];
    ret.e[5] = this->e[2] * other.e[3] + this->e[5] * other.e[4] + this->e[8] * other.e[5];

    ret.e[6] = this->e[0] * other.e[6] + this->e[3] * other.e[7] + this->e[6] * other.e[8];
    ret.e[7] = this->e[1] * other.e[6] + this->e[4] * other.e[7] + this->e[7] * other.e[8];
    ret.e[8] = this->e[2] * other.e[6] + this->e[5] * other.e[7] + this->e[8] * other.e[8];

    return ret;
}

RMatrix3 RMatrix3::operator*(float scalar) const {
    RMatrix3 ret = RMatrix3();

    ret.e[0] = this->e[0] * scalar;
    ret.e[1] = this->e[1] * scalar;
    ret.e[2] = this->e[2] * scalar;
    ret.e[3] = this->e[3] * scalar;
    ret.e[4] = this->e[4] * scalar;
    ret.e[5] = this->e[5] * scalar;
    ret.e[6] = this->e[6] * scalar;
    ret.e[7] = this->e[7] * scalar;
    ret.e[8] = this->e[8] * scalar;

    return ret;
}

RMatrix3& RMatrix3::operator+=(const RMatrix3& other){
    this->e[0] += other.e[0];
    this->e[1] += other.e[1];
    this->e[2] += other.e[2];
    this->e[3] += other.e[3];
    this->e[4] += other.e[4];
    this->e[5] += other.e[5];
    this->e[6] += other.e[6];
    this->e[7] += other.e[7];
    this->e[8] += other.e[8];

    return *this;
}

RMatrix3& RMatrix3::operator-=(const RMatrix3& other){
    this->e[0] -= other.e[0];
    this->e[1] -= other.e[1];
    this->e[2] -= other.e[2];
    this->e[3] -= other.e[3];
    this->e[4] -= other.e[4];
    this->e[5] -= other.e[5];
    this->e[6] -= other.e[6];
    this->e[7] -= other.e[7];
    this->e[8] -= other.e[8];

    return *this;
}

RMatrix3& RMatrix3::operator*=(const RMatrix3& other){
    RMatrix3 temp = RMatrix3();

    temp.e[0] = this->e[0] * other.e[0] + this->e[3] * other.e[1] + this->e[6] * other.e[2];
    temp.e[1] = this->e[1] * other.e[0] + this->e[4] * other.e[1] + this->e[7] * other.e[2];
    temp.e[2] = this->e[2] * other.e[0] + this->e[5] * other.e[1] + this->e[8] * other.e[2];

    temp.e[3] = this->e[0] * other.e[3] + this->e[3] * other.e[4] + this->e[6] * other.e[5];
    temp.e[4] = this->e[1] * other.e[3] + this->e[4] * other.e[4] + this->e[7] * other.e[5];
    temp.e[5] = this->e[2] * other.e[3] + this->e[5] * other.e[4] + this->e[8] * other.e[5];

    temp.e[6] = this->e[0] * other.e[6] + this->e[3] * other.e[7] + this->e[6] * other.e[8];
    temp.e[7] = this->e[1] * other.e[6] + this->e[4] * other.e[7] + this->e[7] * other.e[8];
    temp.e[8] = this->e[2] * other.e[6] + this->e[5] * other.e[7] + this->e[8] * other.e[8];

    *this = temp;
    return *this;
}

RMatrix3& RMatrix3::operator=(const RMatrix3& other){
    this->e[0] = other.e[0];
    this->e[1] = other.e[1];
    this->e[2] = other.e[2];
    this->e[3] = other.e[3];
    this->e[4] = other.e[4];
    this->e[5] = other.e[5];
    this->e[6] = other.e[6];
    this->e[7] = other.e[7];
    this->e[8] = other.e[8];

    return *this;
}

RMatrix3& RMatrix3::operator*=(float scalar){
    this->e[0] *= scalar;
    this->e[1] *= scalar;
    this->e[2] *= scalar;
    this->e[3] *= scalar;
    this->e[4] *= scalar;
    this->e[5] *= scalar;
    this->e[6] *= scalar;
    this->e[7] *= scalar;
    this->e[8] *= scalar;


    return *this;
}


float* RMatrix3::getArray(){
    return this->e;
}

void RMatrix3::loadIdentity(){
    this->e[0] = 1.f;
    this->e[1] = 0.f;
    this->e[2] = 0.f;
    this->e[3] = 0.f;
    this->e[4] = 1.f;
    this->e[5] = 0.f;
    this->e[6] = 0.f;
    this->e[7] = 0.f;
    this->e[8] = 1.f;
}



RMatrix3 RMatrix3::translation(float tx, float ty){
    RMatrix3 ret = RMatrix3();
    ret.loadIdentity();

    ret.e[6] = tx;
    ret.e[7] = ty;

    return ret;
}

RMatrix3 RMatrix3::rotation(float angle){
    RMatrix3 ret = RMatrix3();
    ret.loadIdentity();

    float rcos = cos(angle);
    float rsin = sin(angle);

    ret.e[0] = rcos;
    ret.e[1] = rsin;
    ret.e[3] = -rsin;
    ret.e[4] = rcos;

    return ret;
}

RMatrix3 RMatrix3::scaling(float sx, float sy){
    RMatrix3 ret = RMatrix3();
    ret.loadIdentity();

    ret.e[0]  = sx;
    ret.e[4]  = sy;
    ret.e[8] = 1.f;

    return ret;
}

