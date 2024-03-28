#include "BvhNode.h"

int box_x_compare(const void* a, const void* b) {
	AABB box_left, box_right;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in BvhNode constructor\n";
	if (box_left.min().x() - box_right.min().x() < 0.0f)
		return -1;
	else
		return 1;
}
int box_y_compare(const void* a, const void* b) {
	AABB box_left, box_right;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in BvhNode constructor\n";
	if (box_left.min().y() - box_right.min().y() < 0.0f)
		return -1;
	else
		return 1;
}
int box_z_compare(const void* a, const void* b) {
	AABB box_left, box_right;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in BvhNode constructor\n";
	if (box_left.min().z() - box_right.min().z() < 0.0f)
		return -1;
	else
		return 1;
}

BvhNode::BvhNode(Hitable** l, int n, float time0, float time1, std::mt19937& mt)
{
	int axis = random_unit_int(2, mt); // randomly choses an axis to split the box in two
	auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;
	std::qsort(l, n, sizeof(Hitable*), comparator); // sort boxes with the chosen comparator x-y-z

	if (n == 1) // if there is only one item
		_left = _right = l[0];
	else if (n == 2) { // if there is only two
		_left = l[0];
		_right = l[1];
	} else { // we divide item count by 2 each time since we split tree in two
		_left = new BvhNode(l, n / 2, time0, time1, mt);
		_right = new BvhNode(l + n / 2, n - n / 2, time0, time1, mt);
	}

	AABB box_left, box_right;
	if (!_left->bounding_box(time0, time1, box_left) || !_right->bounding_box(time0, time1, box_right)) // retrieve both children boxes and check if they exist
		std::cerr << "no bounding box in BvhNode constructor\n";
	_box = surrounding_box(box_left, box_right); // create the node's bounding box
}

bool BvhNode::hit(Ray& r, float t_min, float t_max, hit_record& rec, std::mt19937& mt) const
{
	if (_box.hit(r, t_min, t_max)) {
		hit_record left_rec, right_rec;
		bool hit_left = _left->hit(r, t_min, t_max, left_rec, mt); // check left hit recursively
		bool hit_right = _right->hit(r, t_min, t_max, right_rec, mt); // check right hit recursively
		if (hit_left && hit_right) { // check if both boxes got hit
			if (left_rec.t < right_rec.t)
				rec = left_rec;
			else
				rec = right_rec;
			return true;
		}
		else if (hit_left) {
			rec = left_rec;
			return true;
		}
		else if (hit_right) {
			rec = right_rec;
			return true;
		}
	}
	return false;
}

bool BvhNode::bounding_box(float t0, float t1, AABB& b) const
{
	b = _box;
	return true;
}