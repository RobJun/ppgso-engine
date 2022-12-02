#include "Bat.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Bat::mesh;
std::unique_ptr<ppgso::Texture> Bat::texture;
std::unique_ptr<ppgso::Shader> Bat::shader;


Bat::Bat(Scene* scene)
{
	material.diffuse = glm::vec3{ 0.50754,	0.50754,	0.50754 };
	material.ambient = glm::vec3{ 0.3,	0.3,	0.3 };
	material.specular = glm::vec3{ 0.508273, 0.508273, 0.508273 };;
	material.shininess = .4;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/VAMPBAT.TIF"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/VAMP_BAT.obj");
}

bool Bat::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	age += dt;
	if (age > 1)
		return false;
	position += translation * dt;
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene, dt, modelMatrix);
	}
	return true;
}

void Bat::render(Scene& scene)
{
	shader->use();
	//scene.useGlobalLights(shader.get());
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

void Bat::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene,shader);
	}
}

void Bat::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
