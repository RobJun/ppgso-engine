#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

/*!
 * Simple camera object that keeps track of viewMatrix and projectionMatrix
 * the projectionMatrix is by default constructed as perspective projection
 * the viewMatrix is generated from up, position and back vectors on update
 */
class Camera {
public:
	glm::vec3 up{ 0,1,0 };
	glm::vec3 position{ 0,3,-40 };
	glm::vec3 back{ 0,0,-1 };
	glm::vec3 Front{0,0,1};

	glm::vec3 rotate = { 0,0.3,0 };
	glm::vec3 rotation = { 0,0,0 };

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

	glm::vec3 getPosition() { return position; };

	void update(float time);

	glm::vec3 cast(double u, double v);
};