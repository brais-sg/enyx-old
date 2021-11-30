/**
 * @file RVector2.cpp
 * @author Brais Solla González
 * @brief RVector2 implementation
 * @version 0.1
 * @date 2021-11-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "RGLES2/RVector2.h"
#include "RGLES2/RVector2i.h"

#ifndef sq
#define sq(x) (((x)*(x)))
#endif


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