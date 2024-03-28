#ifndef __LAMBERTIAN__
#define __LAMBERTIAN__

#include "Material.h"
#include "Random_unit.h"

class Lambertian : public Material {
	public:
		Lambertian(Texture *a) : _albedo(a) {};
		virtual bool scatter(Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered, std::mt19937& mt) const
		{
			//Lambertian scatter direction is a random vector in a 1:1:1 sphere
			vec3 direction = rec.normal + random_in_unit_sphere(mt);

			scattered = Ray(rec.p, direction); //Create new ray with new direction
			attenuation = _albedo->value(rec.u, rec.v, rec.p); //Gather material attenuation
			return true;
		};
	private:
		Texture *_albedo;
};

#endif

