#ifndef __HITABLE__
#define __HITABLE__

#include "vec3.h"
#include "Ray.h"
#include "Material.h"
#include "AABB.h"

class Hitable {
	public:
		virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec, std::mt19937& mt) const = 0;
		virtual bool bounding_box(float t0, float t1, AABB& box) const = 0;
};

#endif