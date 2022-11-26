#include "Tree2.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Tree2::mesh;
std::unique_ptr<ppgso::Texture> Tree2::texture;
std::unique_ptr<ppgso::Shader> Tree2::shader;


Tree2::Tree2(Scene* scene)
{
	material.diffuse = glm::vec3{ 0.1,	0.35,	0.1 };
	material.ambient = glm::vec3{ 0,	0, 0 };
	material.specular = glm::vec3{ 0.45,	0.55,	0.45 };;
	material.shininess = .25;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/texture_solid.png"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/strom2.obj");
}

bool Tree2::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene,dt,parentModelMatrix);
	}
	return true;
}

void Tree2::render(Scene& scene)
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

void Tree2::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene,shader);
	}
}

void Tree2::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
