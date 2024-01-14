#ifndef __ROTATION__
#define __ROTATION__

#include "Hitable.h"

class RotateY : public Hitable {
	public:
		RotateY(float angle, Hitable* p) : _ptr(p) {
			float radians = (M_PI / 180.) * angle;
			_sinTheta = sin(radians);
			_cosTheta = cos(radians);
			_hasBox = _ptr->bounding_box(0, 1, _bbox);
			vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
			vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
			for (int i = 0; i < 2; i++)
				for (int j = 0; j < 2; j++)
					for (int k = 0; k < 2; k++) {
						float x = i * _bbox.max().x() + (1 - i) * _bbox.min().x();
						float y = j * _bbox.max().y() + (1 - j) * _bbox.min().y();
						float z = k * _bbox.max().z() + (1 - k) * _bbox.min().z();
						float newx = _cosTheta * x + _sinTheta * z;
						float newz = -_sinTheta * x + _cosTheta * z;
						vec3 tester(newx, y, newz);
						for (int c = 0; c < 3; c++) {
							if (tester[c] > max[c])
								max[c] = tester[c];
							if (tester[c] < min[c])
								min[c] = tester[c];
						}
					}
			_bbox = AABB(min, max);
		}
		virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec) const {
			vec3 origin = r.origin();
			vec3 direction = r.direction();
			origin[0] = _cosTheta * r.origin()[0] - _sinTheta * r.origin()[2];
			origin[2] = _sinTheta * r.origin()[0] + _cosTheta * r.origin()[2];
			direction[0] = _cosTheta * r.direction()[0] - _sinTheta * r.direction()[2];
			direction[2] = _sinTheta * r.direction()[0] + _cosTheta * r.direction()[2];
			Ray rotated_r(origin, direction);
			if (_ptr->hit(rotated_r, t_min, t_max, rec)) {
				vec3 p = rec.p;
				vec3 normal = rec.normal;
				p[0] = _cosTheta * rec.p[0] + _sinTheta * rec.p[2];
				p[2] = -_sinTheta * rec.p[0] + _cosTheta * rec.p[2];
				normal[0] = _cosTheta * rec.normal[0] + _sinTheta * rec.normal[2];
				normal[2] = -_sinTheta * rec.normal[0] + _cosTheta * rec.normal[2];
				rec.p = p;
				rec.normal = normal;
				return true;
			}
			return false;
		}
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			box = _bbox;
			return _hasBox;
		}
	private:
		Hitable* _ptr;
		float _sinTheta;
		float _cosTheta;
		bool _hasBox;
		AABB _bbox;
};

#endif