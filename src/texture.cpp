#include <iostream>
#include <stdio.h>
#include <cmath>
#include <stdexcept>

#include "texture.h"
#include "../lib/lodepng/lodepng.h"

Texture::Texture(const char *filename) {
	image = std::vector<unsigned char>();
	unsigned error = lodepng::decode(image, width, height, filename);
	//if there's an error, display it
	printf(">>> Loading texture file %s\n", filename);
	if (error) { std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl << std::endl; return; }
	loaded = true;

}

// Get pixel at texture coordinates x,y
Vec3f Texture::GetPixel(unsigned x, unsigned y) const {

	if (!loaded)
		return (Vec3f(1, 0, 1));

	double r, g, b;
	r = (double)image.at(y*width * 4 + x) / 255.;
	g = (double)image.at(y*width * 4 + x + 1) / 255.;
	b = (double)image.at(y*width * 4 + x + 2) / 255.;
	return Vec3f(r, g, b);
}

// Get pixel at normalised texture coordinates u,v
Vec3f Texture::GetPixel(double u, double v) const {

	if (!loaded)
		return (Vec3f(1, 0, 1));

	int x = (int)(fmod(fabs(u), 1.0)) * (width - 1);
	int y = (int)(1. - fmod(fabs(v), 1.0)) * (height - 1);
	//printf("%f, %f\n", u, v);
	double r, g, b;
	try {
		r = (double)image.at(y*width * 4 + x * 4) / 255.;
		g = (double)image.at(y*width * 4 + x * 4 + 1) / 255.;
		b = (double)image.at(y*width * 4 + x * 4 + 2) / 255.;
		return Vec3f(r, g, b);
	}
	catch (const std::out_of_range& e) {
		printf("Error with UV, yx: %lf, %lf - %i, %i (width, height: %i, %i) \n", u, v, x, y, width, height);
		return Vec3f(0, 1, 0);
	}
}

// Check if texture is loaded
bool Texture::IsLoaded() const {
	return loaded;
}