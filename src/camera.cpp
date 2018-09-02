#include <random>
#include "geometry.h"
#include "camera.h"

std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0, 1);

Vec3f ConcentricDiskSample(const Vec3f &u) {
	// Map uniform random numers to [-1, 1]
	Vec3f uOffset = 2.0 * u - Vec3f(1, 1, 0);

	if (uOffset.x == 0 && uOffset.y == 0)
		return Vec3f(0);

	// Apply concentric mapping
	double theta, r;
	if ((std::abs(uOffset.x) > (std::abs(uOffset.y)))) {
		r = uOffset.x;
		theta = (M_PI / 4.) * (uOffset.y / uOffset.x);
	}
	else {
		r = uOffset.y;
		theta = (M_PI / 2) - (M_PI / 4) * (uOffset.x / uOffset.y);
	}

	return r * Vec3f(std::cos(theta), std::sin(theta), 0);
}


Camera::Camera(Matrix44f c2w, double _fov, double lensRadius, double focalDisance, int width, int height) {

	m_fov = _fov;
	m_cameraToWorld = c2w;
	//m_cameraToWorld.multVecMatrix(Vec3f(0), m_orig);
	m_width = width;
	m_width_recp = 1. / m_width;
	m_height = height;
	m_height_recp = 1. / m_height;
	m_ratio = (double)m_width / m_height;
	m_lensRadius = lensRadius;	// For Depth-Of-Field
	m_focalDistance = focalDisance;	// For Depth-Of-Field

	m_x_direction = Vec3f(0, 0, 1).CrossProduct(m_dir * -1).Normalize();
	m_y_direction = m_x_direction.CrossProduct(m_dir).Normalize();

	m_x_spacing = (2.0 * m_ratio) / (double)m_width;
	m_y_spacing = (double)2.0 / (double)m_height;
	m_x_spacing_half = m_x_spacing * 0.5;
	m_y_spacing_half = m_y_spacing * 0.5;

}

int Camera::GetWidth() { return m_width; }
int Camera::GetHeight() { return m_height; }

// Returns ray from camera origin through pixel at x,y
Ray Camera::GetRay(int _x, int _y, bool jitter, unsigned short *Xi) {

	double imageAspectRatio = m_width / (double)m_height;
	double scale = tan(deg2rad(m_fov * 0.5));
	double x = (2 * (_x + 0.5) / (double)m_width - 1) * imageAspectRatio * scale;
	double y = (1 - 2 * (_y + 0.5) / (double)m_height) * scale;
	Vec3f dir;


	m_x_spacing = (2.0 * imageAspectRatio) / (double)m_width;
	m_y_spacing = (double)2.0 / (double)m_height;

	double x_jitter;
	double y_jitter;

	// If jitter == true, jitter point for anti-aliasing
	if (jitter) {
		//x_jitter = (erand48(Xi) * m_x_spacing) - (m_x_spacing * 0.5);
		//y_jitter = (erand48(Xi) * m_y_spacing) - (m_y_spacing * 0.5);
		x = (2 * (_x + ((2.0 * distribution(generator)) - 1.0)) / (double)m_width - 1) * scale;
		y = (1 - 2 * (_y + ((2.0 * distribution(generator) - 1.0))) / (double)m_height) * scale * 1 / imageAspectRatio;

	}
	else {
		x_jitter = 0;
		y_jitter = 0;
	}

	//x += x_jitter;
	//y += y_jitter;

	Vec3f tempOrig = Vec3f(0);
	Vec3f tempDir = Vec3f(x, y, -1);

	// Modify ray for Depth-Of-Field
	if (m_lensRadius > 0) {
		// Sample point on lens
		Vec3f pLens = m_lensRadius * ConcentricDiskSample(Vec3f(x, y, 0));

		// Compute point on plane of focus
		//double ft = m_focalDistance / -1;
		Vec3f pFocus = tempDir * m_focalDistance;

		//tempOrig = Vec3f(pLens.x, pLens.y, 0);
		//tempDir = Vec3f(pFocus - tempOrig).Normalize();
		//std::cerr << m_focalDistance << std::endl;
	}

	m_cameraToWorld.multVecMatrix(tempOrig, m_orig);
	m_cameraToWorld.multDirMatrix(tempDir, dir);

	dir.Normalize();
	return Ray(m_orig, dir);

}