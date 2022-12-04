#include "camera.h"


Camera::Camera(float fow, float ratio, float near, float far) {
	float fowInRad = (ppgso::PI / 180.0f) * fow;

	projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void Camera::update(float time) {
	back.y = -sin(pitch);
	back.x = -cos(yaw) * cos(pitch);
	back.z = -sin(yaw) * cos(pitch);
	glm::mat4 roll_mat = glm::rotate(glm::mat4(1.0f), roll, -back);
	glm::vec3 temp_up = roll_mat * glm::vec4(up,0);
	
	viewMatrix = lookAt(position,position - back, temp_up);
}		
