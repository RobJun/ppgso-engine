#include "camera.h"


Camera::Camera(float fow, float ratio, float near, float far) {
	float fowInRad = (ppgso::PI / 180.0f) * fow;

	projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void Camera::update(float time) {
	back.y = -sin(pitch);
	back.x = -cos(yaw) * cos(pitch);
	back.z = -sin(yaw) * cos(pitch);
	
	viewMatrix = lookAt(position,position - back, up);
}		
