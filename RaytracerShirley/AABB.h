#ifndef __AABB__
#define __AABB__

#include "Vec3.h"
#include "Ray.h"

class AABB {
	public:
		AABB() {}
		AABB(const vec3& a, const vec3& b) { _min = a; _max = b; }

		vec3 min() const { return _min; }
		vec3 max() const { return _max; }

		bool hit(Ray& r, float t_min, float t_max) const {
			for (int i = 0; i < 3; i++) {
				float invD = 1.0f / r.direction()[i];
				float t0 = (min()[i] - r.origin()[i]) * invD;
				float t1 = (max()[i] - r.origin()[i]) * invD;
				if (invD < 0.0f)
					std::swap(t0, t1);
				t_min = t0 > t_min ? t0 : t_min;
				t_max = t1 < t_max ? t1 : t_max;
				if (t_max <= t_min)
					return false;
			}
			return true;
		}
	private:
		vec3 _min;
		vec3 _max;
};

#endif // !__AABB__

