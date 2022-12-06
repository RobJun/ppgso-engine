#include "campfire.h"

#include <ppgso/ppgso.h>

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>

#include "../objects/particleSystem.h"
#include "./sphere.h"
#include "Light.h"




// Static resources
std::unique_ptr<ppgso::Mesh> Campfire::mesh;
std::unique_ptr<ppgso::Texture> Campfire::texture;
std::unique_ptr<ppgso::Shader> Campfire::shader;


Campfire::Campfire(Scene* scene)
{
	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/tree_bark.jpg"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/logs.obj");

	material.diffuse = glm::vec3{0.5,0.5,0.4};
	material.ambient = glm::vec3{ 0.05,	0.05,	0.0 };
	material.specular = glm::vec3{ 0.7,	0.7,	0.04 };
	material.shininess = .078125;
	material.transparency = 1;

	auto fire = std::make_unique<Light>(scene,"./res/fire.obj");
	fire->color = { 40,4,0 };
	fire->scale = { 1,1,1 };
	children.push_back(move(fire));

	auto particles = std::make_unique<ParticleSystem<Sphere>>(scene);
	particles->position = { 0,0.7,0 };

	ppgso::KeyFrame<glm::vec3> init;
	init.transformTo = { 0.2,0.2,0.2 };
	init.time = 0;
	init.interpolation = ppgso::CONSTANT;
	particles->sizeFrames.addFrame(init);
	ppgso::KeyFrame<glm::vec3> end;
	end.transformTo ={ 0,0,0 };
	end.time = 10;
	end.addInterState({ 0.7,0.7,0.7 },0.8);
	end.interpolation = ppgso::POLYNOMIC;
	particles->sizeFrames.addFrame(end);
	
	particles->numberOfParticles = 2;
	particles->radius = 0.7;
	particles->spawnInterval = 0.3f;
	particles->particleLifeSpan = 10;
	children.push_back(move(particles));

	lightIndex = scene->generatePointLight(glm::vec3{ 0,1,0 }, glm::vec3{ 1.f,1.f,1.f }, glm::vec3{ 1,0.8,0.5 }, glm::vec3{ 0.045, 0.0075, 1});
	scene->enableLight_point(lightIndex);
}

bool Campfire::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	scene.pointLights[lightIndex]->position = position + glm::vec3{0,1.,0};
	for (auto& ch : children) {
		ch->update(scene, dt, modelMatrix);
	}
	return true;
}

void Campfire::render(Scene& scene)
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

void Campfire::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Campfire::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
