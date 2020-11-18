#pragma once

struct Vec3 {//just using as data storage for now
    
public:
    static const Vec3 Zero;


    float x;
    float y;
    float z;

    Vec3(){
        (*this) = Zero; 
    }

    Vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3(const Vec3& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }
};