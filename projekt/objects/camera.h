#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

class Camera {
public:
	glm::vec3 up{ 0,1 ,0 };
	glm::vec3 position{ 0,0,-10 };
	glm::vec3 back{ 0,0,-1 };

	glm::vec3 rotate = { 0,0,0 };			//zapinanie, vypinanie rotovania kamery
	glm::vec3 rotation = { 0,0,0 };

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	float yaw = 0;
	float pitch = 0;


	int lightIndex = -1;
	ppgso::light::LightName name;

	Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

	glm::vec3 getPosition() { return position; };

	virtual void update(float time);
};