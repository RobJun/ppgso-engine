
#include "car.h"
#include "cube.h"
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

	/*auto cube = std::make_unique<Cube>();
	cube->scale = { 0.1,0.1,0.1 };
	cube->position = glm::vec3(0.7,-00.35,2.1) + direction;
	children.push_back(move(cube));*/
	ppgso::light::Spot spot;
	spot.position = { 0,4,3 };
	spot.direction = direction;
	spot.ambient = 0.9f;
	spot.diffuse = 0.8f;
	spot.specular = 1.0f;
	spot.color = { 1,0.8,0.3};
    spot.constant =	 1.0f;
    spot.linear	= 0.22f;
    spot.quadratic= 0.0019f;
	spot.cutOff = glm::radians(12.5f);
	spot.outerCutOff = glm::radians(17.0f);
	left  = scene->addLight(spot);
	right = scene->addLight(spot);
	scene->spotLights[left]->position = modelMatrix * glm::translate(glm::mat4(), { 0.7,-00.35,2.1 }) * glm::vec4(0, 0, 0, 1);
	scene->spotLights[right]->position = modelMatrix * glm::translate(glm::mat4(), { -0.7,-00.35,2.1 }) * glm::vec4(0, 0, 0, 1);
	scene->enableLight_spot(left);
	scene->enableLight_spot(right);
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