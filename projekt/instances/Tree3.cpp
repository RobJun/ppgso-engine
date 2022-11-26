#include "Tree3.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Tree3::mesh;
std::unique_ptr<ppgso::Texture> Tree3::texture;
std::unique_ptr<ppgso::Shader> Tree3::shader;


Tree3::Tree3(Scene* scene)
{
	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/texture_solid.png"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/strom3.obj");
}

bool Tree3::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;

	for (auto& ch : children) {
		ch->update(scene,dt,modelMatrix);
	}
	return true;
}

void Tree3::render(Scene& scene)
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

void Tree3::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();

	for (auto& ch : children) {
		ch->renderMap(scene,shader);
	}
}

void Tree3::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
