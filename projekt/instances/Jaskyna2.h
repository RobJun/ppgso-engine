#pragma once
#include <memory>
#include <tuple>
#include <ppgso/ppgso.h>

#include "../scene.h"
#include "../objects/object.h"


class Jaskyna2 final : public Object {
private:
	static std::unique_ptr<ppgso::Mesh> mesh;
	static std::unique_ptr<ppgso::Shader> shader;
	static std::unique_ptr<ppgso::Texture> texture;

	float degrees = 0;

	float age{ 0.0f };

public:
	float speed = 0.0;
	ppgso::Material material;
	glm::vec3 rotMoSlaganitJaskyna2tum = { 0,0,0.1 };
	glm::vec3 translation = { 0,0,0 };
	glm::vec3 direction = { 0,0,1 };
public:
	Jaskyna2(Scene* scene);

	bool update(Scene& scene, float dt, glm::mat4 parentModelMatrix) override;

	void render(Scene& scene) override;

	void renderMap(Scene& scene, ppgso::Shader* shader) override;

	void renderLights(Scene& scene) override;

};