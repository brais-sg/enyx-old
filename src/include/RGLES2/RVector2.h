/**
 * @file RVector2.h
 * @author Brais Solla González
 * @brief Enyx RGLES2 RVector2 object
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RVECTOR2_INCLUDED
#define _ENYX_RGLES2_RVECTOR2_INCLUDED

#include "RGLES2/RVector2i.h"

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

#endif