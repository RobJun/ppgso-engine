#include "camera.h"


#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include "camera.h"

class CurveCamera : public Camera {
public:

	std::vector<glm::vec3> controlPoints;
	ppgso::Method interpolation = ppgso::BEZIER;
	float age = 0;
	float maxTime = 20;

	CurveCamera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

	glm::vec3 getPosition() { return position; };

	void update(float time);
};