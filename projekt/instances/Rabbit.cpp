#include "Rabbit.h"
#include "Chicken.h"
#include "fox.h"

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
	age += dt;
	if (age > 31)
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


	if (translateFrames.size() - translateFrames.sizeOfAnimations() == 1 && !spawnedFox) {
		spawnedFox = true;
		auto chicken = std::make_unique<Chicken>(&scene);
		chicken->scale = { 0.5,0.5,0.5 };
		chicken->position = { 0,4.5,0 };

		auto fox = std::make_unique<Fox>(&scene);
		fox->scale = { 0.2,0.2,0.2 };
		fox->position = { 10,0,15 };
		fox->rotation = { 0,0,-3.14/2+0.5 };
		fox->children.push_back(move(chicken));

		ppgso::KeyFrame<glm::vec3> firstPosition;
		firstPosition.transformTo = { 10,0,15 };
		firstPosition.time = 0;
		fox->translateFrames.addFrame(firstPosition);

		ppgso::KeyFrame<glm::vec3> finalPosition;
		finalPosition.transformTo = position + glm::vec3{ 3,0,-3 };
		finalPosition.time = 10;
		finalPosition.interpolation = ppgso::LINEAR;
		fox->translateFrames.addFrame(finalPosition);

		finalPosition.transformTo = position + glm::vec3{ 3,0,-3 };
		finalPosition.time = 15;
		finalPosition.interpolation = ppgso::LINEAR;
		fox->translateFrames.addFrame(finalPosition);

		finalPosition.transformTo = { -20,0,25 };
		finalPosition.time = 20;
		finalPosition.interpolation = ppgso::LINEAR;
		fox->translateFrames.addFrame(finalPosition);

		children.push_back(move(fox));
	}

	generateModelMatrix();
	//modelMatrix = parentModelMatrix * modelMatrix;
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
