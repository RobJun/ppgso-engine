#pragma once
#include <memory>
#include <tuple>
#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../objects/object.h"


class Plane final : public Object {
private:
	static std::unique_ptr<ppgso::Mesh> mesh;
	static std::unique_ptr<ppgso::Shader> shader;
	static std::unique_ptr<ppgso::Texture> texture;

	int left, right;
	glm::vec3 direction = { 0,0,1 };

	// Age of the object in seconds
	float age{ 0.0f };

	// Speed and rotational momentum
	glm::vec3 speed;
	glm::vec3 rotMomentum = { 0,0,0.1 };
	glm::vec3 translation = { 0,0,0 };

	glm::vec3 normal = {0,-1,0};
	

public:
	ppgso::Material material;
	Plane(Scene* scene);

	bool update(Scene& scene, float dt, glm::mat4 parentModelMatrix) override;

	/*!
	 * Render asteroid
	 * @param scene Scene to render in
	 */
	void render(Scene& scene) override;

	void renderMap(Scene& scene, ppgso::Shader* shader) override;

	void renderLights(Scene& scene) override;


	glm::vec3 getClosestPoint(glm::vec3 point);

};