#include "sphere.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Sphere::mesh;
std::unique_ptr<ppgso::Texture> Sphere::texture;
std::unique_ptr<ppgso::Shader> Sphere::shader;


Sphere::Sphere(Scene* scene)
{
	material.diffuse = glm::vec3{ 0.5,0.0,0.0 };
	material.ambient = glm::vec3{ 0.55,0.55,	0.55 };
	material.specular = glm::vec3{ 0.7,	0.7,0. };;
	material.shininess = .25;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/black.jpg"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

bool Sphere::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	age += dt;
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
	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene,dt, modelMatrix);
	}
	return true;
}

void Sphere::render(Scene& scene)
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

void Sphere::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Sphere::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
