#ifndef AABBOX_H
#define AABBOX_H

#include <cfloat>
#include <algorithm>

#include "geometry.h"


// Axis Aligned Bounding Box
struct AABBox {
	Vec3f bl; // Bottom left
	Vec3f tr; // Top right

	AABBox(Vec3f _bl = Vec3f(0), Vec3f _tr = Vec3f(0)) {
		bl = _bl, tr = _tr;
	}

	// Expand to fit box
	void Expand(const AABBox &b) {
		if (b.bl.x < bl.x) bl.x = b.bl.x;
		if (b.bl.y < bl.y) bl.y = b.bl.y;
		if (b.bl.z < bl.z) bl.z = b.bl.z;

		if (b.tr.x > tr.x) tr.x = b.tr.x;
		if (b.tr.y > tr.y) tr.y = b.tr.y;
		if (b.tr.z > tr.z) tr.z = b.tr.z;
	}

	// Expand to fit point
	void Expand(const Vec3f &v) {
		if (v.x < bl.x) bl.x = v.x;
		if (v.y < bl.y) bl.y = v.y;
		if (v.z < bl.z) bl.z = v.z;
	}

	// Returns longest axis: 0, 1, 2 for x, y, z respectively
	int GetLongestAxis() {
		Vec3f diff = tr - bl;
		if (diff.x > diff.y && diff.x > diff.z) return 0;
		if (diff.y > diff.x && diff.y > diff.z) return 1;
		return 2;
	}

	// Check if ray intersects with box. Returns true/false and stores distance in t
	bool Intersection(const Ray &r, double &t) {
		double tx1 = (bl.x - r.o.x) * r.dir_inv.x;
		double tx2 = (tr.x - r.o.x) * r.dir_inv.x;

		double tmin = std::min(tx1, tx2);
		double tmax = std::max(tx1, tx2);

		double ty1 = (bl.y - r.o.y) * r.dir_inv.y;
		double ty2 = (tr.y - r.o.y) * r.dir_inv.y;

		tmin = std::max(tmin, std::min(ty1, ty2));
		tmax = std::min(tmax, std::max(ty1, ty2));

		double tz1 = (bl.z - r.o.z) * r.dir_inv.z;
		double tz2 = (tr.z - r.o.z) * r.dir_inv.z;

		tmin = std::max(tmin, std::min(tz1, tz2));
		tmax = std::min(tmax, std::max(tz1, tz2));
		t = tmin;

		return tmax >= tmin;
	}
};

#endif // AABBOX_H