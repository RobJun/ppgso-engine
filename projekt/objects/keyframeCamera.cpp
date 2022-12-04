#include "keyframeCamera.h"


KeyframeCamera::KeyframeCamera(float fow, float ratio, float near, float far) {
	float fowInRad = (ppgso::PI / 180.0f) * fow;

	projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
};



void KeyframeCamera::update(float time) {
	age += time;
	position = k_position.update(age);
	back = k_back.update(age);

	if (!k_position.isOnLastFrame())
		roll = k_roll.update(age);
	else
		roll = 0;
	glm::mat4 roll_mat = glm::rotate(glm::mat4(1.0f), roll, -back);
	glm::vec3 temp_up = roll_mat * glm::vec4(up, 0);

	viewMatrix = lookAt(position, position - back, temp_up);
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

	ppgso::KeyFrame<float> initialUp;
	initialUp.transformTo = roll;
	initialUp.time = 0;

	k_roll.addFrame(initialUp);
}
