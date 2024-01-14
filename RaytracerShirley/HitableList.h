#ifndef __HITABLELIST__
#define __HITABLELIST__

#include "Hitable.h"

AABB surrounding_box(AABB box0, AABB box1);

class HitableList : public Hitable {
public:
	HitableList(Hitable** l, int n);
	virtual bool hit(Ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, AABB& box) const;

private:
	Hitable** _list;
	int _list_size;
};

#endif