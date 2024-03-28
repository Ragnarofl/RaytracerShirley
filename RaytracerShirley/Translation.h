#ifndef __TRANSLATION__
#define __TRANSLATION__

#include "Hitable.h"

class Translation : public Hitable {
	public:
		Translation(const vec3& displacement, Hitable* p) : _offset(displacement), _ptr(p) {}
		virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec, std::mt19937& mt) const {
			// move the ray backward by the offset (change ray from world space to object space)
			Ray moved_r(r.origin() - _offset, r.direction());
			if (_ptr->hit(moved_r, t_min, t_max, rec, mt)) { // test the hit
				// move the intersection point forward by the offset (change intersection from object space to world space)
				rec.p += _offset;
				return true;
			}
			return false;
		}
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			if (_ptr->bounding_box(t0, t1, box)) {
				box = AABB(box.min() + _offset, box.max() + _offset);
				return true;
			}
			return false;
		}
	private:
		vec3 _offset;
		Hitable* _ptr;
};

#endif
