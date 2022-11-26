#include <glm/glm.hpp>
#include <time.h>


struct GeneratorShape {


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
		srand(time(NULL));
		float r = radius * sqrt((rand() % 1000) / 1000.f);
		float t = ((rand() % 1000) / 1000.f) * 2 * 3.14;
		float x = r * cos(t);
		float z = r * sin(t);

		if (up.y > 0) {
			return {x,0,z };
		}
	}
};

struct RectangelGenShape : GeneratorShape {
	glm::vec3 up = { 0,1,0 };
	float A;
	float B;

	RectangelGenShape(float A, float B)
		:  A(A), B(B) {};

	glm::vec3 getRandomPoint() override {
		srand(time(NULL));
		float A_t = A * (rand() % 1000 / 1000.f);
		float B_t = A * (rand() % 1000 / 1000.f);
		float x = A_t - A / 2;
		float z = B_t - B / 2;

		if (up.y > 0) {
			return { x, 0, z };
		}
	}
};