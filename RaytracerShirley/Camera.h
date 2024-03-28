#ifndef __CAMERA__
#define __CAMERA__

#include "Hitable.h"
#include "Random.h"

class Camera {
public:
	Camera();
	Camera(float vfov, float aspect);
	Camera(vec3 lookFrom, vec3 lookAt, vec3 vup, float vfov, float aspect);
	Camera(vec3 lookFrom, vec3 lookAt, vec3 vup, float vfov, float aspect, float aperture, float focus_dist);
	Ray get_ray(float u, float v, std::mt19937 mt);
private:
	vec3 _origin;
	vec3 _lower_left_corner;
	vec3 _horizontal;
	vec3 _vertical;
	//Direction camera is looking at
	vec3 _u, _v, _w;
	float _lens_radius = 0;
};

#endif

