#ifndef __RANDOM__
#define __RANDOM__

#include <random>
#include <map>
#include "Vec3.h"
#include <omp.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Random_unit {
	private:
		explicit Random_unit()
		{
			_mt = std::mt19937(_rd());
			_dist = std::uniform_real_distribution<double>(0.0, 1.0);
			_dist_int;
		};

		std::random_device _rd;
		std::mt19937 _mt;
		std::uniform_real_distribution<double> _dist;
		std::uniform_int_distribution<int> _dist_int;

	public:
		Random_unit(Random_unit& other) = delete;
		void operator=(const Random_unit) = delete;

		static Random_unit* getInstance()
		{
			static Random_unit _randomUnit;
			return &_randomUnit;
		}

		vec3 random_in_unit_sphere()
		{
			float r1 = random_unit_float();
			float r2 = random_unit_float();
			float phi = 2 * M_PI * r1;
			float theta = acos(1 - 2 * r2);
			float x = cos(phi) * sin(theta);
			float y = sin(phi) * sin(theta);
			float z = (1 - 2 * r2);
			vec3 p(x, y, z);

			return p;
		}

		vec3 random_in_unit_sphere(std::mt19937 mt)
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

		vec3 random_in_unit_disk()
		{
			float r1 = random_unit_float();
			float r2 = random_unit_float();
			float phi = 2 * M_PI * r1;
			float theta = acos(1 - 2 * r2);
			float x = 0 + 2 * 1 * cos(phi) * sin(theta) / 2;
			float y = 0 + 2 * 1 * sin(phi) * sin(theta) / 2;
			float z = 0;
			vec3 p(x, y, z);

			return p;
		}

		vec3 random_in_unit_disk(std::mt19937 mt)
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

		float random_unit_float()
		{
			std::uniform_real_distribution<double> dist(0.0, 1.0);
			return dist(_mt);
		}

		float random_unit_float(std::mt19937 mt)
		{
			std::uniform_real_distribution<double> dist(0.0, 1.0);
			return dist(mt);
		}

		int random_unit_int(int max)
		{
			std::uniform_int_distribution<int> dist_int(0, max);
			return dist_int(_mt);
		}

		int random_unit_int(int max, std::mt19937 mt)
		{
			std::uniform_int_distribution<int> dist_int(0, max);
			return dist_int(mt);
		}
};

#endif
