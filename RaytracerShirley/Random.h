#ifndef __RANDOM_UNIT__
#define __RANDOM_UNIT__

#include <random>
#include "Vec3.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float random_unit_float(std::mt19937& mt);
int random_unit_int(int max, std::mt19937& mt);
vec3 random_in_unit_sphere(std::mt19937& mt);
vec3 random_in_unit_disk(std::mt19937& mt);

#endif
