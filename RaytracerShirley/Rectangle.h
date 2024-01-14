#ifndef __RECTANGLE__
#define __RECTANGLE__

#include "Hitable.h"

class xy_rect : public Hitable {
	public:
		xy_rect(float x0, float x1, float y0, float y1, float k, Material* mat) : _x0(x0), _x1(x1), _y0(y0), _y1(y1), _k(k), _mat(mat) {};
		virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec) const {
			float t = (_k - r.origin().z()) / r.direction().z();

			if (t >= t_min && t < t_max) {
				float x = r.origin().x() + t * r.direction().x();
				float y = r.origin().y() + t * r.direction().y();
				if (x < _x0 || x > _x1 || y < _y0 || y > _y1)
					return false;
				rec.u = (x - _x0) / (_x1 - _x0);
				rec.v = (y - _y0) / (_y1 - _y0);
				rec.t = t;
				rec.mat_ptr = _mat;
				rec.p = r.point_at_parameter(t);
				rec.normal = vec3(0, 0, 1);
				return true;
			}
			return false;
		}
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			box = AABB(vec3(_x0, _y0, _k - 0.0001), vec3(_x1, _y1, _k + 0.0001));
			return true;
		}

	private:
		Material* _mat;
		float _x0, _x1, _y0, _y1, _k;
};

class xz_rect : public Hitable {
	public:
		xz_rect(float x0, float x1, float z0, float z1, float k, Material* mat) : _x0(x0), _x1(x1), _z0(z0), _z1(z1), _k(k), _mat(mat) {};
		virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec) const {
			float t = (_k - r.origin().y()) / r.direction().y();

			if (t >= t_min && t < t_max) {
				float x = r.origin().x() + t * r.direction().x();
				float z = r.origin().z() + t * r.direction().z();
				if (x < _x0 || x > _x1 || z < _z0 || z > _z1)
					return false;
				rec.u = (x - _x0) / (_x1 - _x0);
				rec.v = (z - _z0) / (_z1 - _z0);
				rec.t = t;
				rec.mat_ptr = _mat;
				rec.p = r.point_at_parameter(t);
				rec.normal = vec3(0, 1, 0);
				return true;
			}
			return false;
		}
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			box = AABB(vec3(_x0, _k - 0.0001, _z0), vec3(_x1, _k + 0.0001, _z1));
			return true;
		}

	private:
		Material* _mat;
		float _x0, _x1, _z0, _z1, _k;
};

class yz_rect : public Hitable {
	public:
		yz_rect(float y0, float y1, float z0, float z1, float k, Material* mat) : _y0(y0), _y1(y1), _z0(z0), _z1(z1), _k(k), _mat(mat) {};
		virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec) const {
			float t = (_k - r.origin().x()) / r.direction().x();

			if (t >= t_min && t < t_max) {
				float y = r.origin().y() + t * r.direction().y();
				float z = r.origin().z() + t * r.direction().z();
				if (y < _y0 || y > _y1 || z < _z0 || z > _z1)
					return false;
				rec.u = (y - _y0) / (_y1 - _y0);
				rec.v = (z - _z0) / (_z1 - _z0);
				rec.t = t;
				rec.mat_ptr = _mat;
				rec.p = r.point_at_parameter(t);
				rec.normal = vec3(1, 0, 0);
				return true;
			}
			return false;
		}
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			box = AABB(vec3(_k - 0.0001, _y0, _z0), vec3(_k + 0.0001, _y1, _z1));
			return true;
		}

	private:
		Material* _mat;
		float _y0, _y1, _z0, _z1, _k;
};

#endif
