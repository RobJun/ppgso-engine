
#include "water.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Water::mesh;
std::unique_ptr<ppgso::Texture> Water::texture;
std::unique_ptr<ppgso::Shader> Water::shader;

Water::Water(Scene* scene) {
	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/water.png"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");

	material.diffuse = glm::vec3{ 0.1,	0.35,	0.1 };
	material.ambient = glm::vec3{ 0,	0, 0 };
	material.specular = glm::vec3{ 0.45,	0.55,	0.45 };;
	material.shininess = .25;
	material.transparency = 1;
}


bool Water::update(Scene& scene, float dt, glm::mat4 parentModelMatrix) {
	rotation = { glm::radians(270.f),0,0 };
	generateModelMatrix();
	for (auto& ch : children) {
		ch->update(scene, dt, glm::mat4(1));
	}
	return true;
}

void Water::render(Scene& scene) {
	for (auto& ch : children) {
		ch->render(scene);
	}

	shader->use();
	scene.useGlobalLights(shader.get());
	scene.useCamera(shader.get());
	scene.useLights(shader.get());

	material.use(shader.get());
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();

}


void Water::renderMap(Scene& scene, ppgso::Shader* shader) {
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Water::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
