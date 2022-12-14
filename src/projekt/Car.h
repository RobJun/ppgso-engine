#pragma once
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"


class Car final : public Object {
private:
	static std::unique_ptr<ppgso::Mesh> mesh;
	static std::unique_ptr<ppgso::Shader> shader;
	static std::unique_ptr<ppgso::Texture> texture;

	// Age of the object in seconds
	float age{ 0.0f };

	// Speed and rotational momentum
	glm::vec3 speed;
	glm::vec3 rotMomentum = {0,0,0.1};

public:
	Car();

	/*!
	 * Update asteroid
	 * @param scene Scene to interact with
	 * @param dt Time delta for animation purposes
	 * @return
	 */
	bool update(Scene& scene, float dt) override;

	/*!
	 * Render asteroid
	 * @param scene Scene to render in
	 */
	void render(Scene& scene) override;

	
};