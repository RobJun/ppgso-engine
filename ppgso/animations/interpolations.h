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

	glm::vec4 bezierInter(std::vector<glm::vec4> controlPoints, float step);

	glm::vec4 linearInter(std::vector<glm::vec4> controlPoints, float t);

	glm::vec4 polynomialInter(std::vector<glm::vec4> controlPoints, std::vector<float> times, float t);


	glm::vec3 bezierInter(std::vector<glm::vec3> controlPoints, float step);

	glm::vec3 linearInter(std::vector<glm::vec3> controlPoints, float t);

	glm::vec3 polynomialInter(std::vector<glm::vec3> controlPoints, std::vector<float> times, float t);


	glm::vec2 bezierInter(std::vector<glm::vec2> controlPoints, float step);

	glm::vec2 linearInter(std::vector<glm::vec2> controlPoints, float t);

	glm::vec2 polynomialInter(std::vector<glm::vec2> controlPoints, std::vector<float> times, float t);


	float bezierInter(std::vector<float> controlPoints, float step);

	float linearInter(std::vector<float> controlPoints, float t);

	float polynomialInter(std::vector<float> controlPoints, std::vector<float> times, float t);


}

