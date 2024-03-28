#include "Random.h"

float random_unit_float(std::mt19937& mt)
{
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(mt);
}

int random_unit_int(int max, std::mt19937& mt)
{
	std::uniform_int_distribution<int> dist_int(0, max);
	return dist_int(mt);
}

vec3 random_in_unit_sphere(std::mt19937& mt)
{
	float r1 = random_unit_float(mt);
	float r2 = random_unit_float(mt);
	float phi = 2 * M_PI * r1;
	float theta = acos(1 - 2 * r2);
	float x = cos(phi) * sin(theta);
	float y = sin(phi) * sin(theta);
	float z = (1 - 2 * r2);
	vec3 p(x, y, z);

	return p;
}

vec3 random_in_unit_disk(std::mt19937& mt)
{
	float r1 = random_unit_float(mt);
	float r2 = random_unit_float(mt);
	float phi = 2 * M_PI * r1;
	float theta = acos(1 - 2 * r2);
	float x = 0 + 2 * 1 * cos(phi) * sin(theta) / 2;
	float y = 0 + 2 * 1 * sin(phi) * sin(theta) / 2;
	float z = 0;
	vec3 p(x, y, z);

	return p;
}