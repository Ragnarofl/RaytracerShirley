#include "Ray.h"

Ray::Ray() {}

Ray::Ray(const vec3& a, const vec3& b)
{
	_A = a;
	_B = b;
}

vec3 Ray::origin()
{
	return (_A);
}

vec3 Ray::direction()
{
	return (unit_vector(_B));
}

vec3 Ray::point_at_parameter(float t)
{
	return (_A + (t * unit_vector(_B)));
}
