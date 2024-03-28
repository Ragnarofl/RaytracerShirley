#ifndef __SPHERE__
#define __SPHERE__

#include "Hitable.h"
#include "Material.h"

class Sphere : public Hitable {
public:
	Sphere(vec3 cen, float r, Material* m) : _center(cen), _radius(r), _mat_ptr(m) {};

	virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec, std::mt19937& mt) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const;

private:
	vec3 _center;
	float _radius;
	Material* _mat_ptr;
	static void get_sphere_uv(const vec3& p, float& u, float& v);
};

#endif

