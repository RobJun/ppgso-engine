#pragma once
#include <memory>
#include <tuple>
#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../objects/object.h"


class Cube final : public Object {
private:
	static std::unique_ptr<ppgso::Mesh> mesh;
	static std::unique_ptr<ppgso::Shader> shader;
	static std::unique_ptr<ppgso::Texture> texture;

	struct Material {
		float shininess = 32;
		float transparency = 1.f;
	} material;

	// Age of the object in seconds
	float age{ 0.0f };

	// Speed and rotational momentum
	glm::vec3 speed;
	glm::vec3 rotMomentum = { 0,0,0.1 };
	glm::vec3 translation = { 0,0,0 };

public:
	Cube();

	/*!
	 * Update asteroid
	 * @param scene Scene to interact with
	 * @param dt Time delta for animation purposes
	 * @return
	 */
	bool update(Scene& scene, float dt, glm::mat4 parentModelMatrix) override;

	/*!
	 * Render asteroid
	 * @param scene Scene to render in
	 */
	void render(Scene& scene) override;

	void renderMap(Scene& scene, ppgso::Shader* shader) override;

	void renderLights(Scene& scene ) override;

};