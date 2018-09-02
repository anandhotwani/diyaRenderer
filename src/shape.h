#ifndef OBJECTS_H
#define OBJECTS_H

#include "geometry.h"
#include "triangle.h"
#include "kdtree.h"
#include "material.h"
#include "../lib/tiny_obj_loader/tiny_obj_loader.h"

struct IsectInfo {
	bool hit;		// Return intersection state
	double tNear;	// Distance to intersection along ray
	Vec3f n;		// Normal
	Material m;		// Material 

	IsectInfo(bool _hit = false, double _tNear = 0, Vec3f _n = Vec3f(0), Material _m = Material());
};

class Shape {

public:
	Shape(const Matrix44f &o2w) : objectToWorld(o2w), worldToObject(o2w.inverse()) {}
	Vec3f m_pos; // Position
	virtual IsectInfo GetIntersection(const Ray &r) = 0;

	Matrix44f objectToWorld, worldToObject;
};

class Sphere : public Shape {

private:
	double m_rad;	// Radius
	Material m_mat;	// Material

public:
	Sphere(const Matrix44f &o2w, double r_, Material m_) : Shape(o2w), m_rad(r_), m_mat(m_) { o2w.multVecMatrix(Vec3f(0), m_pos); }
	virtual double GetRadius();
	virtual Material GetMaterial();
	virtual IsectInfo GetIntersection(const Ray &r);

};

class TriangleMesh : public Shape {

private:
	std::vector<tinyobj::shape_t> m_shapes;
	std::vector<tinyobj::material_t> m_materials;
	std::vector<Material> materials;
	std::vector<Triangle*> tris;
	Material m_mat;	// Material
	KDNode *node;

public:
	TriangleMesh(const Matrix44f &o2w, const char* file_path, Material m_);
	virtual IsectInfo GetIntersection(const Ray &r);

};


#endif // OBJECTS_H