/**
 * @file RVector2i.h
 * @author Brais Solla González
 * @brief Enyx RGLES2 RVector2i object
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _ENYX_RGLES2_RVECTOR2I_INCLUDED
#define _ENYX_RGLES2_RVECTOR2I_INCLUDED

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

#endif