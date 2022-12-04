#include "Fox.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Fox::mesh;
std::unique_ptr<ppgso::Texture> Fox::texture;
std::unique_ptr<ppgso::Shader> Fox::shader;


Fox::Fox(Scene* scene)
{
	material.ambient = glm::vec3{ 0.2,.2,.2 };
	material.diffuse = glm::vec3{ 0.2,.2,.2 };
	material.specular = glm::vec3{ 0.2,.2,.2 };
	material.shininess = .25;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/foxy.png"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/foxy.obj");
}

bool Fox::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	age += dt;
	if (age > 23)
		return false;
	if (!sizeFrames.isEmpty()) {
		scale = sizeFrames.update(age);
	}
	else {
		///nieco ine
	}
	if (!translateFrames.isEmpty()) {
		position = translateFrames.update(age);
	}
	else {
		///nieco ine
	}

	if (!rotationFrames.isEmpty()) {
		rotation = rotationFrames.update(age);
	}
	else {
		///nieco ine
	}

	generateModelMatrix();
	//modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene, dt, modelMatrix);
	}
	return true;
}

void Fox::render(Scene& scene)
{
	shader->use();
	scene.useGlobalLights(shader.get());
	scene.useCamera(shader.get());
	scene.useLights(shader.get());

	material.use(shader.get());
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	glDisable(GL_CULL_FACE);
	mesh->render();
	glEnable(GL_CULL_FACE);

	for (auto& ch : children) {
		ch->render(scene);
	}
}

void Fox::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Fox::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
