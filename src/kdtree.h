#ifndef KDTREE_H
#define KDTREE_H

#include <vector>

#include "geometry.h"
#include "triangle.h"

class KDNode {
public:
	AABBox box;
	KDNode* left;
	KDNode* right;
	std::vector<Triangle*> triangles;
	bool leaf;

	KDNode() {};
	KDNode* Build(std::vector<Triangle*> &tris, int depth);
	bool Hit(KDNode* node, const Ray &ray, double &t, double &tmin, Vec3f &normal, Vec3f &c);
};

#endif // KDTREE_H