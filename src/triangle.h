#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <algorithm>
#include <cfloat>

#include "geometry.h"
#include "aabbox.h"
#include "material.h"

class Triangle {

public:
	Triangle(Vec3f _v0, Vec3f _v1, Vec3f _v2, Vec3f _t0 = Vec3f(), Vec3f _t1 = Vec3f(), Vec3f _t2 = Vec3f(), Material *_m = NULL) {
		v0 = _v0, v1 = _v1, v2 = _v2, e1 = v1 - v0, e2 = v2 - v0, n = e1.CrossProduct(e2).Normalize();
		t0 = _t0, t1 = _t1, t2 = _t2;
		m = _m;
	}

	// Returns axis aligned bounding box that contains the triangle
	AABBox GetBoundingBox() {
		Vec3f bl = Vec3f(
			std::min(std::min(v0.x, v1.x), v2.x),
			std::min(std::min(v0.y, v1.y), v2.y),
			std::min(std::min(v0.z, v1.z), v2.z)
		);
		Vec3f tr = Vec3f(
			std::max(std::max(v0.x, v1.x), v2.x),
			std::max(std::max(v0.y, v1.y), v2.y),
			std::max(std::max(v0.z, v1.z), v2.z)
		);

		return AABBox(bl, tr);
	}

	// Returns the midpoint of the triangle
	Vec3f GetMidpoint() {
		return (v0 + v1 + v2) / 3;
	}

	// Checks if ray intersects with triangle. Returns true/false.
	bool Intersect(Ray ray, double &t, double tmin, Vec3f &norm) const {

		double u, v, t_temp = 0;

		Vec3f pvec = ray.d.CrossProduct(e2);
		double det = e1.DotProduct(pvec);
		if (det == 0) return false;
		double invDet = 1. / det;
		Vec3f tvec = ray.o - v0;
		u = tvec.DotProduct(pvec) * invDet;
		if (u < 0 || u > 1) return false;
		Vec3f qvec = tvec.CrossProduct(e1);
		v = ray.d.DotProduct(qvec) * invDet;
		if (v < 0 || u + v > 1) return false;
		t_temp = e2.DotProduct(qvec) * invDet; // Set distance along ray to intersection
		if (t_temp < tmin) {
			if (t_temp > 1e-9) {    // Fairly arbritarily small value, scared to change
				t = t_temp;         // it as it works.
				norm = n;
				return true;
			}
		}
		return false;
	}

	// Returns barycentric coordinates of point p on the triangle
	Vec3f Barycentric(Vec3f p) {
		Vec3f v2_ = p - v0;
		double d00 = e1.DotProduct(e1);
		double d01 = e1.DotProduct(e2);
		double d11 = e2.DotProduct(e2);
		double d20 = v2_.DotProduct(e1);
		double d21 = v2_.DotProduct(e2);
		double d = d00 * d11 - d01 * d01;
		double v = (d11*d20 - d01 * d21) / d;
		double w = (d00*d21 - d01 * d20) / d;
		double u = 1 - v - w;
		return Vec3f(u, v, w);
	}

	// Returns the colour at point p on the triangle
	Vec3f GetColorAt(Vec3f p) {
		if (m == NULL) return Vec3f(1, 0, 1);

		Vec3f b = Barycentric(p);
		Vec3f c = Vec3f(0);
		c = c + (t0 * b.x);
		c = c + (t1 * b.y);
		c = c + (t2 * b.z);

		return m->GetColorAt(c.x, c.y);
	}

	Vec3f v0, v1, v2;     // Vertex world space coordinates
	Vec3f e1, e2;         // Edge 1 and edge 2 of triangle
	Vec3f n, t0, t1, t2;  // Triangle normal and texture coordinates
	Material *m;        // Pointer to material

};

#endif // TRIANGLE_H