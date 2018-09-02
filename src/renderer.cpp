#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <omp.h>
#include "renderer.h"
#include "../lib/lodepng/lodepng.h"

// Clamp double to min/max of 0/1
inline double clamp(double x) { return x < 0 ? 0 : x > 1 ? 1 : x; }
// Clamp to between 0-255
inline int toInt(double x) { return int(clamp(x) * 255 + .5); }

Vec3f RRTAndODTFit(Vec3f v) {

	Vec3f a = v * (v + 0.0245786f) - 0.000090537f;
	Vec3f b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
	return Vec3f((a.x / b.x), (a.y / b.y), (a.z / b.z));

}

// Curve fitted approximation by Stephen Hill (@self_shadow). Good appoximation though slightly oversaturates compared to CTL transform.
Vec3f ACESFitted(Vec3f color) {

	color.x = color.x * 0.59719 + color.y * 0.35458 + color.z * 0.04823;
	color.y = color.x * 0.07600 + color.y * 0.90834 + color.z * 0.01566;
	color.z = color.x * 0.02840 + color.y * 0.13383 + color.z * 0.83777;

	color = RRTAndODTFit(color);

	color.x = color.x * 1.60475 + color.y * -0.53108 + color.z * -0.07367;
	color.y = color.x * -0.10208 + color.y *  1.10813 + color.z * -0.00605;
	color.z = color.x * -0.00327 + color.y * -0.07276 + color.z *  1.07602;

	return color;
}


inline double ApplysRGBCurve(double x)
{
	return x < 0.0031308 ? 12.92 * x : 1.055 * pow(x, 1.0 / 2.4) - 0.055;
}

inline double ApplyRec709Curve(double x)
{
	return x < 0.0181 ? 4.5 * x : 1.0993 * pow(x, 0.45) - 0.0993;
}


Renderer::Renderer(Scene *scene, Camera *camera) {
	m_scene = scene;
	m_camera = camera;

	m_pixel_buffer = new Vec3f[m_camera->GetWidth()*m_camera->GetHeight()];
}

void Renderer::Render(int samples) {
	int width = m_camera->GetWidth();
	int height = m_camera->GetHeight();
	double samples_recp = 1. / samples;

	// Main render loop
#pragma omp parallel for schedule(dynamic, 1)       // OpenMP for simple multithreading
	for (int y = 0; y<height; ++y) {
		unsigned short Xi[3] = { 0,0,y*y*y };               // Stores seed for erand lib

		fprintf(stderr, "\r>>> Rendering scene with %i spp: %.2f%% ",
			samples, (double)y / height * 100);

		for (int x = 0; x<width; ++x) {

			Vec3f col = Vec3f(0);	// Set default colour to black

			for (int a = 0; a<samples; ++a) {
				Ray ray = m_camera->GetRay(x, y, a>0, Xi);	// Camera jitter value goes here
				col = col + m_scene->CastRay(ray, 0, Xi);	// Accumulate color
			}
			m_pixel_buffer[(y)*width + x] = col * samples_recp;
		}
	}
}

void Renderer::WriteImage(const char *file_path) {

	int width = m_camera->GetWidth();
	int height = m_camera->GetHeight();

	std::vector<unsigned char> pixel_buffer;

	int pixel_count = width * height;

	//For PPM
	//std::ofstream ofs;
	//ofs.open("out.ppm");
	//ofs<<"P6\n"<<width<<" "<<height<<"\n255\n";
	double gamma = 2.2;

	for (int i = 0; i < pixel_count; ++i) {
		
//ACES tonemap
#if 1
		Vec3f temp = Vec3f(m_pixel_buffer[i].x, m_pixel_buffer[i].y, m_pixel_buffer[i].z);
		temp = ACESFitted(temp);

		pixel_buffer.push_back(toInt(ApplysRGBCurve(temp.x)));
		pixel_buffer.push_back(toInt(ApplysRGBCurve(temp.y)));
		pixel_buffer.push_back(toInt(ApplysRGBCurve(temp.z)));
		pixel_buffer.push_back(255);
#else
// sRGB curve
		pixel_buffer.push_back(toInt(ApplysRGBCurve(m_pixel_buffer[i].x)));
		pixel_buffer.push_back(toInt(ApplysRGBCurve(m_pixel_buffer[i].y)));
		pixel_buffer.push_back(toInt(ApplysRGBCurve(m_pixel_buffer[i].z)));
		pixel_buffer.push_back(255);
#endif
		// For PPM
		//char r = (char)(255 * clamp(pow(m_pixel_buffer[i].x, 1 / gamma)));
		//char g = (char)(255 * clamp(pow(m_pixel_buffer[i].y, 1 / gamma)));
		//char b = (char)(255 * clamp(pow(m_pixel_buffer[i].z, 1 / gamma)));
		//ofs<<r<<g<<b;

	}
	//ofs.close();

	//Encode the image
	unsigned error = lodepng::encode(file_path, pixel_buffer, width, height);
	//Error check
	if (error) std::cout << "Encoding error " << error << ": " << lodepng_error_text(error) << std::endl;

	pixel_buffer.clear();
}
