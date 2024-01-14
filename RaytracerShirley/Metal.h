#ifndef __METAL__
#define __METAL__

#include "Material.h"

class Metal : public Material {
public:
	Metal(const vec3& a, float f) : _albedo(a) { if (f < 1) _fuzz = f; else _fuzz = 1; };
	virtual bool scatter(Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const
	{
		vec3 reflected = r_in.direction() - 2 * (rec.normal * dot(r_in.direction(), rec.normal));
		scattered = Ray(rec.p, reflected + _fuzz * _rdu->random_in_unit_sphere());
		attenuation = _albedo;

		return (dot(scattered.direction(), rec.normal) > 0);
	};
private:
	vec3 _albedo;
	float _fuzz; //Material blurriness value from 0 to 1
	Random_unit* _rdu = Random_unit::getInstance();
};

#endif
