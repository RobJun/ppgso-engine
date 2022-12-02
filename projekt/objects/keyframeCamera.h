#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include "camera.h"

class KeyframeCamera : public Camera {
public:

	ppgso::KeyFrames<glm::vec3> k_position;
	ppgso::KeyFrames<glm::vec3> k_back;
	ppgso::KeyFrames<glm::vec3> k_up;

	float age = 0;

	KeyframeCamera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

	glm::vec3 getPosition() { return position; };

	void update(float time);

	void addInitialKeyframes();
};