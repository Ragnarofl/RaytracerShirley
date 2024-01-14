#include "Camera.h"

Camera::Camera()
{
	_origin = vec3(0, 0, 0);
	_lower_left_corner = vec3(-2, -2, -1);
	_horizontal = vec3(4, 0, 0);
	_vertical = vec3(0, 4, 0);
}

Camera::Camera(float vfov, float aspect)
{
	float theta = vfov * M_PI / 180.0f;
	float half_height = tan(theta / 2.0f);
	float half_width = aspect * half_height;

	_origin = vec3(0, 0, 0);
	_lower_left_corner = vec3(-half_width, -half_height, -1.0f);
	_horizontal = vec3(2 * half_width, 0, 0);
	_vertical = vec3(0, 2 * half_height, 0);
}

Camera::Camera(vec3 lookFrom, vec3 lookAt, vec3 vup, float vfov, float aspect)
{
	vec3 u, v, w;
	float theta = vfov * M_PI / 180.0f;
	float half_height = tan(theta / 2.0f);
	float half_width = aspect * half_height;
	w = unit_vector(lookFrom - lookAt);
	u = unit_vector(cross(vup, w));
	v = cross(w, u);

	_origin = lookFrom;
	_lower_left_corner = (_origin - w) - half_width * u - half_height * v;
	_horizontal = 2.0f * half_width * u;
	_vertical = 2.0f * half_height * v;
}

Camera::Camera(vec3 lookFrom, vec3 lookAt, vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
{
	_lens_radius = aperture / 2;

	float theta = vfov * M_PI / 180.0f;
	float half_height = tan(theta / 2.0f);
	float half_width = aspect * half_height;
	_w = unit_vector(lookFrom - lookAt);
	_u = unit_vector(cross(vup, _w));
	_v = cross(_w, _u);

	_origin = lookFrom;
	_lower_left_corner = _origin - focus_dist * _w - half_width * focus_dist * _u - half_height * focus_dist * _v;
	_horizontal = 2.0f * half_width * focus_dist * _u;
	_vertical = 2.0f * half_height * focus_dist * _v;
}

Ray Camera::get_ray(float u, float v)
{
	/*vec3 p(_lower_left_corner + u * _horizontal + v * _vertical);

	Ray ray(_origin, p - _origin);
	return ray;*/
	vec3 random = _lens_radius * _rdu->random_in_unit_disk();
	vec3 offset = _u * random.x() + _v * random.y();
	vec3 p = _lower_left_corner + u * _horizontal + v * _vertical;

	return Ray(_origin + offset, p - (_origin + offset));
}