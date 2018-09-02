#include "scene.h"
#include "shape.h"

void Scene::Add(Shape *object) {
	m_objects.push_back(object);
}

IsectInfo Scene::Intersect(const Ray &ray) {
	IsectInfo isect = IsectInfo();
	IsectInfo temp;
	long size = m_objects.size();

	for (int i = 0; i < size; ++i) {
		temp = m_objects.at((unsigned)i)->GetIntersection(ray);

		if (temp.hit) {
			if (isect.tNear == 0 || temp.tNear < isect.tNear) isect = temp;
		}
	}
	return isect;
}

Vec3f Scene::CastRay(const Ray &ray, int depth, unsigned short*Xi) {

	IsectInfo isect = Intersect(ray);

	// If no hit, return world colour
	if (!isect.hit)
		return Vec3f();

	if (isect.m.GetType() == EMISSIVE) return isect.m.GetEmission();	// Light

	Vec3f c = isect.m.GetColor();

	// Max reflection
	double p = c.x > c.y && c.x > c.z ? c.x : c.y > c.z ? c.y : c.z;

	// Russian roulette termination.
	// If random number between 0 and 1 is > p, terminate and return hit object's emmission
	double rnd = erand48(Xi);
	if (++depth>5) {
		if (rnd < p * 0.9) { // Multiply by 0.9 to avoid infinite loop with colours of 1.0
			c *= (0.9 / p);
		}
		else {
			return isect.m.GetEmission();
		}
	}

	Vec3f x = ray.o + ray.d * isect.tNear;
	Ray reflected = isect.m.GetReflectedRay(ray, x, isect.n, Xi);

	return c * (CastRay(reflected, depth, Xi));
}
