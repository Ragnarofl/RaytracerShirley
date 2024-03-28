#ifndef __BOX__
#define __BOX__

#include "HitableList.h"
#include "Rectangle.h"
#include "FlipNormals.h"

class Box : public Hitable {
	public:
		Box(const vec3& p0, const vec3& p1, Material* ptr) : _pmin(p0), _pmax(p1) {
			Hitable** list = new Hitable * [6];
			list[0] = new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
			list[1] = new FlipNormals(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
			list[2] = new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
			list[3] = new FlipNormals(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
			list[4] = new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
			list[5] = new FlipNormals(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
			_list_ptr = new HitableList(list, 6);
		};
		virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec, std::mt19937& mt) const {
			return _list_ptr->hit(r, t_min, t_max, rec, mt);
		}
		virtual bool bounding_box(float t0, float t1, AABB& box) const {
			box = AABB(_pmin, _pmax);
			return true;
		}
	private:
		vec3 _pmin, _pmax;
		Hitable* _list_ptr;
};

#endif
