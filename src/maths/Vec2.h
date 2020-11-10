#pragma once

#include <cmath>

struct Vec2 {
    static const Vec2 Zero;


    float x;
    float y;

    Vec2(){
        (*this) = Zero; 
    }

    Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vec2(const Vec2& v) {
        this->x = v.x;
        this->y = v.y;
    }

    inline Vec2 operator+(const Vec2 v) const {
        return Vec2(this->x+v.x,this->y+v.y);
    }
    inline void operator+=(const Vec2 v) {
        this->x += v.x;
        this->y += v.y;
    }
    inline Vec2 operator+(float s) const {
        return Vec2(this->x+s,this->y+s);
    }
    inline void operator+=(float s) {
        this->x += s;
        this->y += s;
    }


    inline Vec2 operator-(const Vec2 v) const {
        return Vec2(this->x-v.x,this->y-v.y);
    }
    inline void operator-=(const Vec2 v) {
        this->x -= v.x;
        this->y -= v.y;
    }
    inline Vec2 operator-(float s) const {
        return Vec2(this->x-s,this->y-s);
    }
    inline void operator-=(float s) {
        this->x -= s;
        this->y -= s;
    }


    inline Vec2 operator*(const float s) const {
        return Vec2(this->x*s,this->y*s);
    }
    inline void operator*=(const float s) {
        this->x *= s;
        this->y *= s;
    }

    inline Vec2 operator/(const float s) const {
        return Vec2(this->x/s,this->y/s);
    }
    inline void operator/=(const float s) {
        this->x /= s;
        this->y /= s;
    }

    inline float mag() const {
        return sqrtf((x*x)+(y*y));
    }

    inline Vec2 normal() const {
        return (*this)/mag();
    }

    inline float sqrMag() const {
        return x*x+y*y;
    }

    inline static float dot(const Vec2 v, const Vec2 w){
        return (v.x*w.x) + (v.y*w.y);
    }

    inline static float dist(const Vec2 v, const Vec2 w){
        Vec2 u = v-w;
        return u.mag();
    }

    inline static float distSqrd(const Vec2 v, const Vec2 w){
        Vec2 u = v-w;
        return u.sqrMag();
    }

    inline Vec2 projOnto(const Vec2 v) const{
        return v*(dot(v,*this)/dot(v,v));
    }

    inline float compIn(const Vec2 v) const{
        return (dot(v,*this)/v.mag());
    }
};