#ifndef __DIELECTRIC__
#define __DIELECTRIC__

#include "Material.h"

float schlick(float cosine, float ref_idx)
{
	//Schlick approxmiation for reflection + refraction
	float r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
	r0 = r0 * r0; //Optimisation, pow is slower than doing square directly
	return (r0 + (1 - r0) * pow(1 - cosine, 5));
}

bool refract(const vec3& v, const vec3& n, float n1_over_n2, vec3& refracted)
{
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0f - (n1_over_n2 * n1_over_n2) * (1.0f - (dt * dt));
	if (discriminant > 0.0f) {
		//Calculate refracted
		refracted = n1_over_n2 * (uv - dt * n) - (sqrt(discriminant) * n);
		return true;
	}
	//Total reflection
	return false;
}

class Dielectric : public Material
{
public:
	Dielectric(float ri) : _ref_index(ri) {};
	virtual bool scatter(Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered, std::mt19937& mt) const
	{
		vec3 outward_normal;
		vec3 refracted;
		float n1_over_n2;
		float reflected_prob;
		float cosine;
		attenuation = vec3(1.0, 1.0, 1.0); //This material absorbs nothing

		//When ray shoot through object back into air
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal; //Flip the normal
			n1_over_n2 = _ref_index; // _ref_index / 1.0
			cosine = _ref_index * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		//When ray shoots into objects
		else {
			outward_normal = rec.normal;
			n1_over_n2 = 1.0 / _ref_index;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}

		//Check if there is refraction
		if (refract(r_in.direction(), outward_normal, n1_over_n2, refracted))
			reflected_prob = schlick(cosine, _ref_index);
		else
			reflected_prob = 1.0; //Total reflection

		if (random_unit_float(mt) < reflected_prob) { //Randomly chose between reflection/refraction to return
			vec3 reflected = r_in.direction() - 2 * (rec.normal * dot(r_in.direction(), rec.normal));
			scattered = Ray(rec.p, reflected);
		}
		else {
			scattered = Ray(rec.p, refracted);
		}

		return true;
	};

private:
	float _ref_index;
};

#endif
