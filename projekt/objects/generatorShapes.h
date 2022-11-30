#include <glm/glm.hpp>
#include <time.h>
#include <vector>


struct GeneratorShape {
	std::vector<glm::vec3> usedPoints;
	float distance = 1.5;

	virtual glm::vec3 getRandomPoint() {
		return glm::vec3(0);
	}
};


struct CircleGenShape : GeneratorShape {
	glm::vec3 up = { 0,1,0 };
	float radius;

	CircleGenShape(float radius)
		: radius(radius) {};

	glm::vec3 getRandomPoint() override {
		float x, z;
		bool existsClose = false;
		do {
			float r = radius * sqrt((rand() % 1000) / 1000.f);
			float t = ((rand() % 1000) / 1000.f) * 2 * 3.14;
			 x = r * cos(t);
			 z = r * sin(t);
			existsClose = false;
			for (auto& p : usedPoints) {
				if (glm::distance(p, { x,0,z }) < distance) {
					existsClose = true;
					break;
				}
			}

		} while (existsClose);


		usedPoints.push_back({ x,0,z });
		if (up.y > 0) {
			return {x,0,z };
		}
	}
};

struct SphereGenShape : GeneratorShape {
	float radius;

	SphereGenShape(float radius)
		: radius(radius) {}

	glm::vec3 getRandomPoint() override {
		float x,y, z;
		bool existsClose = false;
		do {
			float r = radius * sqrt((rand() % 1000) / 1000.f);
			x = ((rand() % 1000)) - 500;
			y = ((rand() % 1000)) - 500;
			z = ((rand() % 1000)) - 500;
			float t = 0;
			if (!(x == 0 && y == 0 && z == 0))
				t = 1 / sqrt(x * x + y * y + z * z);
			x *= t * r;
			y *= t * r;
			z *= t * r;

			existsClose = false;
			for (auto& p : usedPoints) {
				if (glm::distance(p, { x,y,z }) < distance) {
					existsClose = true;
					break;
				}
			}


		} while (existsClose);

		usedPoints.push_back({ x,y,z });
		return { x,y,z };
	}
};

struct RectangelGenShape : GeneratorShape {
	glm::vec3 up = { 0,1,0 };
	float A;
	float B;

	RectangelGenShape(float A, float B)
		:  A(A), B(B) {};

	glm::vec3 getRandomPoint() override {
		float x, y, z;
		bool existsClose = false;
		do {
			float A_t = A * (rand() % 1000 / 1000.f);
			float B_t = B * (rand() % 1000 / 1000.f);
			x = A_t - A / 2;
			z = B_t - B / 2;

			existsClose = false;
			for (auto& p : usedPoints) {
				if (glm::distance(p, { x,0,z }) < distance) {
					existsClose = true;
					break;
				}
			}
		} while (existsClose);



		usedPoints.push_back({ x,0,z });
		if (up.y > 0) {
			return { x, 0, z };
		}
	}
};