#pragma once
#include <memory>
#include <string>
#include <tuple>
#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../objects/object.h"

typedef struct LIGHTCONFIG
{
	glm::vec3 position = { 0,0,0 };
	glm::vec3 color = { 10.0f,5,5 };
} LightConfig;

class Light final : public Object {
	std::unique_ptr<ppgso::Mesh> mesh;
	static std::unique_ptr<ppgso::Shader> shader;

	int lightIndex = -1;
	float degrees = 0;


public:
	// Speed and rotational momentum
	float speed = 0.0;
	glm::vec3 rotMomentum = { 0,0,0.1 };
	glm::vec3 translation = { 0,0,0 };
	glm::vec3 color = { 10.0f,5,5 };
	glm::vec3 direction = { 0,0,1 };

public:
	Light(Scene* scene, const std::string  mesh_filepath,LightConfig config);
	Light(Scene* scene, const std::string  mesh_filepath);

	bool update(Scene& scene, float dt, glm::mat4 parentModelMatrix) ;

	void render(Scene& scene) ;

	void renderMap(Scene& scene, ppgso::Shader* shader);


	void renderLights(Scene& scene) override;
};