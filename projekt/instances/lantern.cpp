#include "lantern.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Lantern::mesh;
std::unique_ptr<ppgso::Texture> Lantern::texture;
std::unique_ptr<ppgso::Shader> Lantern::shader;


Lantern::Lantern(Scene* scene)
{
	material.ambient = glm::vec3{ 0.19125,	0.0735,	0.0225 };
	material.diffuse = glm::vec3{ 0.7038,	0.27048,	0.0828 };
	material.specular = glm::vec3{ 0.256777,	0.137622,	0.086014 };;
	material.shininess = .1;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/lantern_Base_Color.jpg"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/lantern.obj");
}

bool Lantern::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene,dt, modelMatrix);
	}
	return true;
}

void Lantern::render(Scene& scene)
{
	shader->use();
	scene.useGlobalLights(shader.get());
	scene.useCamera(shader.get());
	scene.useLights(shader.get());

	material.use(shader.get());
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();

	for (auto& ch : children) {
		ch->render(scene);
	}
}

void Lantern::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Lantern::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
