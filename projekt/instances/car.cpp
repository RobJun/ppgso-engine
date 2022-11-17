
#include "car.h"
#include "cube.h"
#include "Light.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Car::mesh;
std::unique_ptr<ppgso::Texture> Car::texture;
std::unique_ptr<ppgso::Shader> Car::shader;

Car::Car(Scene* scene) {
	// Initialize static resources if needed
	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/CarTexture.png"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/BasicCar.obj");

	left = scene->generateSpotLight(glm::vec3( 0,4,3 ), direction, glm::vec4{ 1,0.8,0.3,1.f }, glm::vec3{ 0.8f,0.9f,1.0f }, glm::vec3{ 0.22,0.0019,1.f }, glm::vec2{ glm::radians(12.5f) ,glm::radians(17.0f) });
	right = scene->generateSpotLight(glm::vec3(0, 4, 3), direction, glm::vec4{ 1,0.8,0.3,1.f }, glm::vec3{ 0.8f,0.9f,1.0f }, glm::vec3{ 0.22,0.0019,1.f }, glm::vec2{ glm::radians(12.5f) ,glm::radians(17.0f) });
	scene->spotLights[left]->position = modelMatrix * glm::translate(glm::mat4(), { 0.7,-00.35,2.1 }) * glm::vec4(0, 0, 0, 1);
	scene->spotLights[right]->position = modelMatrix * glm::translate(glm::mat4(), { -0.7,-00.35,2.1 }) * glm::vec4(0, 0, 0, 1);
	scene->enableLight_spot(left);
	scene->enableLight_spot(right);

	std::unique_ptr<Light> left_light = std::make_unique<Light> (scene, "cube.obj");
	std::unique_ptr<Light> right_light = std::make_unique<Light>(scene, "cube.obj");
	left_light->position = { 0.7,-00.35,2.1 };
	left_light->color = glm::vec3{ 1,0.8,0.3 }*100.f;
	left_light->scale = right_light->scale = { 0.1,0.1,0.1 };
	right_light->position = { -0.7,-00.35,2.1 };
	right_light->color = glm::vec3{ 1,0.8,0.3 }*100.f;

	children.push_back(move(left_light));
	children.push_back(move(right_light));

	std::unique_ptr<Light> left_light_b = std::make_unique<Light>(scene, "cube.obj");
	std::unique_ptr<Light> right_light_b = std::make_unique<Light>(scene, "cube.obj");
	left_light_b->position = { 0.7,-00.35,-2.1 };
	left_light_b->color = glm::vec3{ 1,0.0,0.0 }*50.f;
	left_light_b->scale = right_light_b->scale = { 0.1,0.1,0.1 };
	right_light_b->position = { -0.7,-00.35,-2.1 };
	right_light_b->color = glm::vec3{ 1,0.0,0.0 }*50.f;

	children.push_back(move(left_light_b));
	children.push_back(move(right_light_b));
}


bool Car::update(Scene& scene, float dt, glm::mat4 parentModelMatrix) {
	rotation -= rotMomentum * dt*10.f;
	position = { position.x,0.75,position.z + speed };
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	glm::vec3 d_direction = glm::rotate(glm::mat4(), rotation.z, glm::vec3(0, 1, 0)) * glm::vec4(direction, 1);
	scene.spotLights[left]->position = modelMatrix * glm::translate(glm::mat4(), { 0.8,-00.35,2.1 }) * glm::vec4(0, 0, 0, 1);
	scene.spotLights[right]->position = modelMatrix * glm::translate(glm::mat4(), { -0.8,-00.35,2.1 }) * glm::vec4(0, 0, 0, 1);
	scene.spotLights[left]->direction = d_direction;
	scene.spotLights[right]->direction = d_direction;
	//std::get<1>(scene.pointLights[left])->position =  glm::vec4(scene.m_camera->position,1) ;
	//std::get<1>(scene.pointLights[left])->direction = glm::vec4(0,0,1,0);
	for (auto& ch : children) {
		ch->update(scene, dt, modelMatrix);
	}
	return true;
}

void Car::render(Scene& scene) {
	shader->use();
	scene.useGlobalLights(shader.get());
	scene.useCamera(shader.get());
	scene.useLights(shader.get());
	shader->setUniform("material.shininess", material.shininess);
	shader->setUniform("material.transparency", material.transparency);
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();

	for (auto& ch : children) {
		ch->render(scene);
	}
}


void Car::renderMap(Scene& scene, ppgso::Shader* shader) {
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
}

void Car::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
