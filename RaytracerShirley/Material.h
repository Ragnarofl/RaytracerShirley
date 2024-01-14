#ifndef __MATERIAL__
#define __MATERIAL__

#include "vec3.h"
#include "Ray.h"
#include "Texture.h"
#include "Random_unit.h"

struct hit_record;

class Material {
    public:
        virtual bool scatter(Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const = 0;
        virtual vec3 emitted(float u, float v, const vec3& p) const {
            return vec3(0, 0, 0);
        }
};

struct hit_record
{
    float t;
    float u;
    float v;

    vec3 p;
    vec3 normal;
    Material* mat_ptr;
};

#endif