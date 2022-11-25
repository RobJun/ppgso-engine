#pragma once

#include <glm/glm.hpp>
#include <vector>


namespace ppgso {

	enum Method {
		CONSTANT,
		LINEAR,
		BEZIER,
		POLYNOMIC,
		DELETE,
	};

	glm::vec3 __bezierRecursicve(std::vector<glm::vec3> points, int j, const float t, const int n);

	extern glm::vec3 bezierInter(std::vector<glm::vec3> controlPoints, float step);

	glm::vec3 linearInter(std::vector<glm::vec3> controlPoints, float t);

	glm::vec3 polynomialInter(std::vector<glm::vec3> controlPoints, std::vector<float> times, float t);

}

