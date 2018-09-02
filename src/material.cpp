#include <stdlib.h>
#include <random>

#include "../src/geometry.h"
#include "../src/material.h"

void CreateLocalCoordinateSystem(const Vec3f &N, Vec3f &Nt, Vec3f &Nb)
{
	if (std::fabs(N.x) > std::fabs(N.y))
		Nt = Vec3f(N.z, 0, -N.x) / sqrt(N.x * N.x + N.z * N.z);
	else
		Nt = Vec3f(0, -N.z, N.y) / sqrt(N.y * N.y + N.z * N.z);

	Nb = N.CrossProduct(Nt);
}

Vec3f UniformSampleHemisphere(const double &r1, const double &r2)
{
	double sinTheta = sqrt(1 - r1 * r1);
	double phi = 2 * M_PI * r2;
	double x = sinTheta * cos(phi);
	double z = sinTheta * sin(phi);

	return Vec3f(x, r1, z);
}

Material::Material(MaterialType t, Vec3f c, Vec3f e, Texture tex) {
	m_type = t, m_colour = c, m_emission = e;
	m_texture = tex;
}

MaterialType Material::GetType() const { return m_type; }
Vec3f Material::GetColor() const { return m_colour; }

// Get UV coords
Vec3f Material::GetColorAt(double u, double v) const {
	if (m_texture.IsLoaded())
		return m_texture.GetPixel(u, v);

	return m_colour;
}

// Light values
Vec3f Material::GetEmission() const { return m_emission; }

Ray Material::GetReflectedRay(const Ray &r, Vec3f &p, const Vec3f &n, unsigned short *Xi) const {

	// Mirror specular 
	if (m_type == SPECULAR) {

		double roughness = 0.1;

		Vec3f reflected = r.d - n * 2 * n.DotProduct(r.d);

		reflected = Vec3f(
			reflected.x + (erand48(Xi) - 0.5) * roughness,
			reflected.y + (erand48(Xi) - 0.5) * roughness,
			reflected.z + (erand48(Xi) - 0.5) * roughness
		).Normalize();

		return Ray(p, reflected);
	}
	// Diffuse reflection
	if (m_type == DIFFUSE) {

		Vec3f N = n.DotProduct(r.d) < 0 ? n : n * -1;

		//Calc spherical coordinates
		double r1 = 2 * M_PI*erand48(Xi);
		double r2 = erand48(Xi);
		double r2s = sqrt(r2);

		Vec3f Nt, Nb;

		CreateLocalCoordinateSystem(N, Nt, Nb);

		// Outgoing direction
		Vec3f d = (Nt * cos(r1) * r2s + Nb * sin(r1) * r2s + N * sqrt(1 - r2)).Normalize();

		return Ray(p, d);
	}
}