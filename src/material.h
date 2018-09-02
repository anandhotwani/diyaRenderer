#ifndef MATERIAL_H
#define MATERIAL_H

#include "../lib/rand48/erand48.h"
#include "geometry.h"
#include "texture.h"



enum MaterialType { DIFFUSE, SPECULAR, EMISSIVE };


class Material {

private:
	MaterialType m_type;
	Vec3f m_colour;
	Vec3f m_emission;
	Texture m_texture;


public:
	Material(MaterialType t = DIFFUSE, Vec3f c = Vec3f(0.18), Vec3f e = Vec3f(0, 0, 0), Texture tex = Texture());
	MaterialType GetType() const;
	Vec3f GetColor() const;
	Vec3f GetColorAt(double u, double v) const;
	Vec3f GetEmission() const;
	Ray GetReflectedRay(const Ray &r, Vec3f &p, const Vec3f &n, unsigned short *Xi) const;

};


#endif // MATERIAL_H