#include "keyframeCamera.h"


KeyframeCamera::KeyframeCamera(float fow, float ratio, float near, float far) {
	float fowInRad = (ppgso::PI / 180.0f) * fow;

	projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
};



void KeyframeCamera::update(float time) {
	age += time;
	position = k_position.update(age);
	back = k_back.update(age);
	up = k_up.update(age);

	viewMatrix = lookAt(position, position - back, up);
}

void KeyframeCamera::addInitialKeyframes() {
	ppgso::KeyFrame<glm::vec3> initialPosition;
	initialPosition.transformTo = position;
	initialPosition.time = 0;
	k_position.addFrame(initialPosition);

	ppgso::KeyFrame<glm::vec3> initialBack;
	initialBack.transformTo = back;
	initialBack.time = 0;
	k_back.addFrame(initialBack);

	ppgso::KeyFrame<glm::vec3> initialUp;
	initialUp.transformTo = up;
	initialUp.time = 0;

	k_up.addFrame(initialUp);
}
