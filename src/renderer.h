#ifndef RENDERER_H
#define RENDERER_H

#include "geometry.h"
#include "scene.h"
#include "camera.h"

class Renderer {

private:
	Scene * m_scene;
	Camera *m_camera;
	Vec3f *m_pixel_buffer;

public:
	Renderer(Scene *scene, Camera *camera);
	void Render(int samples = 4); 
	void WriteImage(const char * file_path);

};

#endif //RENDERER_H