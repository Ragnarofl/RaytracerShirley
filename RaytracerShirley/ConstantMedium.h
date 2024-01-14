#ifndef __CONSTANTMEDIUM__
#define __CONSTANTMEDIUM__

#include "Hitable.h"
#include "Random_unit.h"

class Isotropic : public Material {
	public:
		Isotropic(Texture* a) : _albedo(a) {}
		virtual bool scatter(Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const {
			scattered = Ray(rec.p, _rdu->random_in_unit_sphere());
			attenuation = _albedo->value(rec.u, rec.v, rec.p);
			return true;
		}
	private:
		Texture* _albedo;
		Random_unit* _rdu = Random_unit::getInstance();
};

class ConstantMedium : public Hitable {
	public:
		ConstantMedium(Hitable *b, float d, Texture *a) : _boundary(b), _density(d) {
			_phaseFunction = new Isotropic(a);
		}
		virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec) const {
			bool enableDebug = false; // Print occasional samples when debugging. To enable, set enableDebug true.
			bool db = enableDebug && (_rdu->random_unit_float() < 0.0001);

			hit_record rec1, rec2;
			if (_boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) {
				if (_boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2)) {
					if (db) std::cerr << "\nt0 t1" << rec1.t << " " << rec2.t << "\n";

					if (rec1.t < t_min)
						rec1.t = t_min;
					if (rec2.t > t_max)
						rec2.t = t_max;
					if (rec1.t >= rec2.t)
						return false;
					if (rec1.t < 0)
						rec1.t = 0;

					//The ray can scatter at any point, so we randomise that evental scatter distance.
					float distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
					float hit_distance = -(1 / _density) * log(_rdu->random_unit_float());
					if (hit_distance < distance_inside_boundary) {
						rec.t = rec1.t + hit_distance / r.direction().length();
						rec.p = r.point_at_parameter(rec.t);
						if (db) {
							std::cerr << "hit_distance = " << hit_distance << '\n'
									  << "rec.t = " << rec.t << '\n'
									  << "rec.p = " << rec.p << '\n';
						}
						rec.normal = vec3(1, 0, 0); //arbitrary
						rec.mat_ptr = _phaseFunction;
						return true;
					}
				}
			}
			return false;
		}
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			return _boundary->bounding_box(t0, t1, box);
		}
	private:
		Hitable* _boundary;
		float _density;
		Material* _phaseFunction;
		Random_unit* _rdu = Random_unit::getInstance();
};

#endif
