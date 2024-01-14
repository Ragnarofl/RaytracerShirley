#ifndef __RAY__
#define __RAY__

#include "vec3.h"

class Ray
{
public:
    Ray();
    Ray(const vec3& a, const vec3& b);
    vec3 origin();  //return start position A
    vec3 direction(); //return direction vector B (MUST BE NORMALIZED!)
    vec3 point_at_parameter(float t);  //get a position on the ray at distance t

    vec3 _A; //starting point
    vec3 _B; //direction vector (must be an unit vector)
};

#endif

