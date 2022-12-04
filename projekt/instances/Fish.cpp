#include "Fish.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Fish::mesh;
std::unique_ptr<ppgso::Texture> Fish::texture;
std::unique_ptr<ppgso::Shader> Fish::shader;


Fish::Fish(Scene* scene)
{
	rotation = {0,0,0};
	scale = {.3,.3,.3 };

	material.diffuse = glm::vec3{ 0.4,	0.5,	0.4 };
	material.ambient = glm::vec3{0.5,.5,.5};
	material.specular = glm::vec3{1,1,1};;
	material.shininess = .1;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/fish.png"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/fish.obj");
}

bool Fish::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	vyskakuj(scene, dt);
	
	modelMatrix =
		glm::translate(glm::mat4(1.0f), position)
		* glm::orientate4(rotMomentum)
		* glm::translate(glm::mat4(1.0f), { 0,3,0 })
		* glm::orientate4(rotation)
		* glm::scale(glm::mat4(1.0f), scale);

	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene, dt, modelMatrix);
	}
	return true;
}

void Fish::render(Scene& scene)
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

void Fish::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Fish::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}

void Fish::vyskakuj(Scene& s, float dt) {
	float speed = 3;
	rotMomentum += speed * dt * glm::vec3{ 0,1,0 };
}