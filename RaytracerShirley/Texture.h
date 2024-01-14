#ifndef __TEXTURE__
#define __TEXTURE__

#include "Vec3.h"
#define M_PI 3.14159265358979323846

class Texture {
	public:
		virtual vec3 value(const vec3& p) const = 0;
		virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class Constant_texture : public Texture {
	public:
		Constant_texture() {};
		Constant_texture(vec3 c) : _color(c) {};
		virtual vec3 value(const vec3& p) const {
			return _color;
		}
		virtual vec3 value(float u, float v, const vec3& p) const {
			return _color;
		}
	private:
		vec3 _color;
};

class Checker_texture : public Texture {
	public:
		Checker_texture(Texture* t0, Texture* t1) : _even(t0), _odd(t1) {};
		virtual vec3 value(const vec3& p) const {
			float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
			if (sines < 0)
				return _odd->value(p);
			else
				return _even->value(p);
		}
		virtual vec3 value(float u, float v, const vec3& p) const {
			float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
			if (sines < 0)
				return _odd->value(p);
			else
				return _even->value(p);
		}
	private:
		Texture *_even; //bright tile
		Texture *_odd;  //dark tile
};

class Image_texture : public Texture {
	public:
		Image_texture(unsigned char *pixels, int A, int B) : _data(pixels), _nx(A), _ny(B) {}
		virtual vec3 value(const vec3& p) const {
			return vec3(0, 0, 0);
		}
		virtual vec3 value(float u, float v, const vec3& p) const {
			int i = (u) * _nx;
			int j = (1 - v) * _ny;

			if (i < 0) i = 0;
			if (j < 0) j = 0;
			if (i > _nx - 1) i = _nx - 1;
			if (j > _ny - 1) j = _ny - 1;

			float r = int(_data[3 * i + 3 * _nx * j]) / 255.0;
			float g = int(_data[3 * i + 3 * _nx * j + 1]) / 255.0;
			float b = int(_data[3 * i + 3 * _nx * j + 2]) / 255.0;
			return vec3(r, g, b);
		}
	private:
		unsigned char* _data;
		int _nx, _ny;
};

#endif
