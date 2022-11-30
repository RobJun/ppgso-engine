#include "Rabbit.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Rabbit::mesh;
std::unique_ptr<ppgso::Texture> Rabbit::texture;
std::unique_ptr<ppgso::Shader> Rabbit::shader;


Rabbit::Rabbit(Scene* scene)
{
	material.ambient = glm::vec3{0.5,.5,.5};
	material.diffuse = glm::vec3{ 0.5,.5,.5 };
	material.specular = glm::vec3{ 0.5,.5,.5 };
	material.shininess = .25;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/rabbit_tex.png"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/rabbit.obj");
}

bool Rabbit::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene,dt,modelMatrix);
	}
	return true;
}

void Rabbit::render(Scene& scene)
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

void Rabbit::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Rabbit::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
