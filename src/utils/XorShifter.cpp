#include "XorShifter.h"

int XorShifter::next() {
    unsigned int t = (x ^ (x << 11));
    x = y;
    y = z;
    z = w;
    w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    unsigned int rtn = w & 0x7FFFFFFF;
    if (rtn == 0x7FFFFFFF)
        return next();
    return (int)rtn;
}

float XorShifter::fnext() {
    unsigned int t = (x ^ (x << 11));
    x = y;
    y = z;
    z = w;
    w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    unsigned int rtn = w & 0x7FFFFFFF;
    if (rtn == 0x7FFFFFFF)
        return fnext();
    return rtn / (float)0x7FFFFFFF;
}