#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <ppgso/ppgso.h>

class Scene;



class Object {
public: 
	// Object properties
	glm::vec3 position{ 0,0,0 };
	glm::vec3 rotation{ 0,0,0 };
	glm::vec3 scale{ 1,1,1 };
	glm::mat4 modelMatrix{ 1 };
	std::vector<std::unique_ptr<Object>> children;
	float age{ 0.0f };

	ppgso::KeyFrames sizeFrames;
	ppgso::KeyFrames translateFrames;
	ppgso::KeyFrames rotationFrames;

public:
	Object() {};
	virtual ~Object() {};

	virtual bool update(Scene& scene,float dt, glm::mat4 parentModelMatrix = glm::mat4(1)) = 0;

	virtual void render(Scene& scene) = 0;
	virtual void renderMap(Scene& scene, ppgso::Shader* shader) = 0;

	virtual void renderLights(Scene& scene) = 0;

protected: 
	void generateModelMatrix();
}; 