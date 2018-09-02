#ifndef TEXTURE_H
#define TEXTURE_H
#include <vector>
#include "geometry.h"

class Texture {
private:
	unsigned width;
	unsigned height;
	bool loaded = false;
	std::vector<unsigned char> image;
public:
	Texture(const char* filename);
	Texture() {};
	Vec3f GetPixel(unsigned x, unsigned y) const;
	Vec3f GetPixel(double u, double v) const;
	bool IsLoaded() const;
};

#endif // TEXTURE_H