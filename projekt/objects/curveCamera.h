#include "camera.h"


#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include "camera.h"

class CurveCamera : public Camera {
public:

	std::vector<std::vector<glm::vec3>> controlPoints;
	unsigned int currentCurve = 0;
	ppgso::Method interpolation = ppgso::BEZIER;
	float age = 0;
	float startTime = 0;
	std::vector<float> maxTime;

	CurveCamera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

	glm::vec3 getPosition() { return position; };

	void update(float time);
	void onKey(unsigned int key);
};