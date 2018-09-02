#include <vector>

#include "geometry.h"
#include "triangle.h"
#include "kdtree.h"

// Acceleration structure
KDNode* KDNode::Build(std::vector<Triangle*> &tris, int depth) {
	KDNode* node = new KDNode();
	node->leaf = false;
	node->triangles = std::vector<Triangle*>();
	node->left = NULL;
	node->right = NULL;
	node->box = AABBox();

	if (tris.size() == 0) return node;

	if (depth > 25 || tris.size() <= 6) {
		node->triangles = tris;
		node->leaf = true;
		node->box = tris[0]->GetBoundingBox();

		for (long i = 1; i<tris.size(); i++) {
			node->box.Expand(tris[i]->GetBoundingBox());
		}

		node->left = new KDNode();
		node->right = new KDNode();
		node->left->triangles = std::vector<Triangle*>();
		node->right->triangles = std::vector<Triangle*>();

		return node;
	}

	node->box = tris[0]->GetBoundingBox();
	Vec3f midpt = Vec3f(0);
	double tris_recp = 1.0 / tris.size();

	for (long i = 1; i<tris.size(); i++) {
		node->box.Expand(tris[i]->GetBoundingBox());
		midpt = midpt + (tris[i]->GetMidpoint() * tris_recp);
	}

	std::vector<Triangle*> left_tris;
	std::vector<Triangle*> right_tris;
	int axis = node->box.GetLongestAxis();

	for (long i = 0; i<tris.size(); i++) {
		switch (axis) {
		case 0:
			midpt.x >= tris[i]->GetMidpoint().x ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
			break;
		case 1:
			midpt.y >= tris[i]->GetMidpoint().y ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
			break;
		case 2:
			midpt.z >= tris[i]->GetMidpoint().z ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
			break;
		}
	}

	if (tris.size() == left_tris.size() || tris.size() == right_tris.size()) {
		node->triangles = tris;
		node->leaf = true;
		node->box = tris[0]->GetBoundingBox();

		for (long i = 1; i<tris.size(); i++) {
			node->box.Expand(tris[i]->GetBoundingBox());
		}

		node->left = new KDNode();
		node->right = new KDNode();
		node->left->triangles = std::vector<Triangle*>();
		node->right->triangles = std::vector<Triangle*>();

		return node;
	}

	node->left = Build(left_tris, depth + 1);
	node->right = Build(right_tris, depth + 1);

	return node;
}

// Finds nearest triangle in kd tree that intersects with ray.
bool KDNode::Hit(KDNode *node, const Ray &ray, double &t, double &tmin, Vec3f &normal, Vec3f &c) {
	double dist;
	if (node->box.Intersection(ray, dist)) {
		if (dist > tmin) return false;

		bool hit_tri = false;
		bool hit_left = false;
		bool hit_right = false;
		long tri_idx;

		if (!node->leaf) {
			hit_left = Hit(node->left, ray, t, tmin, normal, c);

			hit_right = Hit(node->right, ray, t, tmin, normal, c);

			return hit_left || hit_right;
		}
		else {
			long triangles_size = node->triangles.size();
			for (long i = 0; i<triangles_size; i++) {
				if (node->triangles[i]->Intersect(ray, t, tmin, normal)) {
					hit_tri = true;
					tmin = t;
					tri_idx = i;
				}
			}
			if (hit_tri) {
				Vec3f p = ray.o + ray.d * tmin;
				c = node->triangles[tri_idx]->GetColorAt(p);
				return true;
			}
		}
	}
	return false;
}