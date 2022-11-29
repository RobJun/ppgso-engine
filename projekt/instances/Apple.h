#pragma once
#include <memory>
#include <tuple>
#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../objects/object.h"


class Apple final : public Object {
private:
	static std::unique_ptr<ppgso::Mesh> mesh;
	static std::unique_ptr<ppgso::Shader> shader;
	static std::unique_ptr<ppgso::Texture> texture;

	float degrees = 0;

	// Age of the object in seconds
	float age{ 0.0f };
	float weight = 1000;
	glm::vec3 velocity = { 0,0,0 };
	bool bounced = false;
	bool stopMovement = false;
public:
	// Speed and rotational momentum
	float speed = 0.0;
	glm::vec3 rotMomentum = { 0,0,0.1 };
	glm::vec3 translation = { 0,0,0 };
	glm::vec3 direction = { 0,0,1 };
	ppgso::Material material;
public:
	Apple(Scene* scene);

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

	void renderLights(Scene& scene) override;


	void padaj(Scene& scene,float dt);

	void collide(Scene& scene);

};