#ifndef __DIFFUSE__
#define __DIFFUSE__

#include "Material.h"

class Diffuse_light : public Material {
	public:
		Diffuse_light(Texture *a) : _emit(a) {}
		virtual bool scatter(Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const {
			return false;
		}
		virtual bool scatter(Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered, std::mt19937& mt) const {
			return false;
		}
		virtual vec3 emitted(float u, float v, const vec3& p) const {
			return _emit->value(u, v, p);
		}
	private:
		Texture *_emit;
};

#endif
