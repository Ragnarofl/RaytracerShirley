#ifndef __BVHNODE__
#define __BVHNODE__

#include "Hitable.h"

AABB surrounding_box(AABB box0, AABB box1);

class BvhNode : public Hitable {
	public:
		BvhNode() {}
		BvhNode(Hitable** l, int n, float time0, float time1, std::mt19937& mt);
		virtual bool hit(Ray& r, float t_min, float t_max, hit_record& rec, std::mt19937& mt) const;
		virtual bool bounding_box(float t0, float t1, AABB& box) const;
	
	private:
		Hitable* _left;
		Hitable* _right;
		AABB _box;
};

#endif
