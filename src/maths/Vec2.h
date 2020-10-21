#pragma once

struct Vec2 {
    
public:
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

    static float mag(const Vec2 v);
    static float sqrMag(const Vec2 v);

    Vec2 operator+(const Vec2 v) const;
    Vec2 operator-(const Vec2 v) const;
    Vec2 operator*(const Vec2 v) const;
    Vec2 operator/(const Vec2 v) const;
    Vec2 operator+(const float v) const;
    Vec2 operator-(const float v) const;
    Vec2 operator*(const float v) const;
    Vec2 operator/(const float v) const;

    static float dist(const Vec2 v, const Vec2 w);
    static float distSquared(const Vec2 v, const Vec2 w);
    static float dot(const Vec2 v, const Vec2 w);
    static float proj(const Vec2 v, const Vec2 w);
};