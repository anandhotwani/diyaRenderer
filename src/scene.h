#ifndef SCENE_H
#define SCENE_H

#include "geometry.h"
#include "shape.h"

class Scene {
private:
	std::vector<Shape*> m_objects;

public:
	Scene() {};
	void Add(Shape *object);
	IsectInfo Intersect(const Ray &ray);
	Vec3f CastRay(const Ray &ray, int depth, unsigned short*Xi);
};

#endif //SCENE_H