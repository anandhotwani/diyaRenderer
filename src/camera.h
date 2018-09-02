#ifndef CAMERA_H
#define CAMERA_H
#include "../lib/rand48/erand48.h"
#include "geometry.h"

class Camera {

private:

	Matrix44f m_cameraToWorld;
	Vec3f m_orig;
	double m_fov;

	// For Depth-Of-Field
	double m_lensRadius;
	double m_focalDistance;

	int m_width;
	double m_width_recp;

	int m_height;
	double m_height_recp;

	double m_ratio;

	double m_x_spacing;
	double m_x_spacing_half;

	double m_y_spacing;
	double m_y_spacing_half;

	Vec3f m_pos;
	Vec3f m_dir;

	Vec3f m_x_direction;
	Vec3f m_y_direction;

public:
	Camera(Matrix44f c2w, double _fov, double lensRadius, double focalDistance, int width, int height);
	int GetWidth();
	int GetHeight();
	Ray GetRay(int x, int y, bool jitter, unsigned short *Xi);

};

#endif //CAMERA_H