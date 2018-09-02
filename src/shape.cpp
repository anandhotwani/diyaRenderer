#include <math.h>
#include <iostream>
#include <stdio.h>

#include "geometry.h"
#include "material.h"
#include "shape.h"


IsectInfo::IsectInfo(bool _hit, double _tNear, Vec3f _n, Material _m)
{
	hit = _hit, tNear = _tNear, n = _n, m = _m;
}

double Sphere::GetRadius() { return m_rad; }
Material Sphere::GetMaterial() { return m_mat; }

// Check if ray intersects with sphere. Returns ObjectIntersection data structure
IsectInfo Sphere::GetIntersection(const Ray &ray) {

	bool hit = false;
	double distance = 0;
	Vec3f n = Vec3f(0);

	Vec3f op = m_pos - ray.o;

	double t;
	double epsilon = 1e-4;
	double b = op.DotProduct(ray.d);
	double discr = b * b - op.DotProduct(op) + m_rad * m_rad;

	if (discr<0) return IsectInfo(hit, distance, n, m_mat);
	else discr = sqrt(discr);
	distance = (t = b - discr)>epsilon ? t : ((t = b + discr)>epsilon ? t : 0);
	if (distance != 0) hit = true,
		n = ((ray.o + ray.d * distance) - m_pos).Normalize();

	return IsectInfo(hit, distance, n, m_mat);
}

TriangleMesh::TriangleMesh(const Matrix44f &o2w, const char* file_path, Material _m): Shape(o2w) {

	o2w.multVecMatrix(Vec3f(0), m_pos);
	m_mat = _m;

	std::string mtlbasepath;
	std::string inputfile = file_path;
	unsigned long pos = inputfile.find_last_of("/");
	mtlbasepath = inputfile.substr(0, pos + 1);

	printf(">>> Parsing .obj file %s...\n", file_path);
	// Attempt to load mesh
	std::string err = tinyobj::LoadObj(m_shapes, m_materials, inputfile.c_str(), mtlbasepath.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}
	printf(">>> Constructing Acceleration Structure (KD-Tree)...\n\n");

	long shapes_size, indices_size, materials_size;
	shapes_size = m_shapes.size();
	materials_size = m_materials.size();

	// Load materials/textures from obj
	for (int i = 0; i<materials_size; ++i) {
		std::string texture_path = "";

		if (!m_materials[i].diffuse_texname.empty()) {
			if (m_materials[i].diffuse_texname[0] == '/') texture_path = m_materials[i].diffuse_texname;
			texture_path = mtlbasepath + m_materials[i].diffuse_texname;
			materials.push_back(Material(DIFFUSE, Vec3f(1, 1, 1), Vec3f(), texture_path.c_str()));
		}
		else {
			materials.push_back(Material(DIFFUSE, Vec3f(1, 1, 1), Vec3f()));
		}

	}

	// Load triangles from obj
	for (int i = 0; i < shapes_size; ++i) {
		indices_size = m_shapes[i].mesh.indices.size() / 3;
		for (size_t f = 0; f < indices_size; ++f) {

			// Triangle vertex coordinates
			Vec3f v0_ = Vec3f(
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3 + 1],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3 + 2]
			) + m_pos;

			Vec3f v1_ = Vec3f(
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 1],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 2]
			) + m_pos;

			Vec3f v2_ = Vec3f(
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 1],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 2]
			) + m_pos;

			Vec3f v0New, v1New, v2New;	// To store object-to-world matrix values

			Vec3f t0_, t1_, t2_;

			//Attempt to load triangle texture coordinates
			if (m_shapes[i].mesh.indices[3 * f + 2] * 2 + 1 < m_shapes[i].mesh.texcoords.size()) {
				t0_ = Vec3f(
					m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f] * 2],
					m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f] * 2 + 1],
					0
				);

				t1_ = Vec3f(
					m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 1] * 2],
					m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 1] * 2 + 1],
					0
				);

				t2_ = Vec3f(
					m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 2] * 2],
					m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 2] * 2 + 1],
					0
				);
			}
			else {
				t0_ = Vec3f();
				t1_ = Vec3f();
				t2_ = Vec3f();
			}
			objectToWorld.multVecMatrix(v0_, v0New);
			objectToWorld.multVecMatrix(v1_, v1New);
			objectToWorld.multVecMatrix(v2_, v2New);

			if (m_shapes[i].mesh.material_ids[f] < materials.size())
				tris.push_back(new Triangle(v0New, v1New, v2New, t0_, t1_, t2_, &materials[m_shapes[i].mesh.material_ids[f]]));
			else
				tris.push_back(new Triangle(v0New, v1New, v2New, t0_, t1_, t2_, &m_mat));
		}
	}

	// Free memory
	m_shapes.clear();
	m_materials.clear();
	node = KDNode().Build(tris, 0);
	printf("\n");
}

// Check if ray intersects with mesh. Returns ObjectIntersection data structure
IsectInfo TriangleMesh::GetIntersection(const Ray &ray) {
	double t = 0, tmin = INFINITY;
	Vec3f normal = Vec3f(0);
	Vec3f colour = Vec3f(0);
	bool hit = node->Hit(node, ray, t, tmin, normal, colour);
	//return IsectInfo(hit, tmin, normal, Material(DIFFUSE, colour, Vec3f()));
	return IsectInfo(hit, tmin, normal, m_mat);

}
