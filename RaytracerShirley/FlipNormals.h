#ifndef __FLIPNORMALS__
#define __FLIPNORMALS__

#include "Hitable.h"

class FlipNormals : public Hitable {
	public:
		FlipNormals(Hitable *p) : _ptr(p) {}
		virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec, std::mt19937& mt) const {
			if (_ptr->hit(r, t_min, t_max, rec, mt)) {
				rec.normal = -rec.normal;
				return true;
			}
			else
				return false;
		}
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			return _ptr->bounding_box(t0, t1, box);
		}
	private:
		Hitable* _ptr;
};

#endif
