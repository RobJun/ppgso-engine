
#include "cube.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Cube::mesh;
std::unique_ptr<ppgso::Texture> Cube::texture;
std::unique_ptr<ppgso::Shader> Cube::shader;

Cube::Cube() {
	// Initialize static resources if needed
	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/CarTexture.png"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");
}


bool Cube::update(Scene& scene, float dt, glm::mat4 parentModelMatrix) {
	//scale = { 10,10,10 };
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	return true;
}

void Cube::render(Scene& scene) {
	shader->use();
	// Set up light
	//shader->setUniform("LightDirection", scene.m_globalLight.direction);
	scene.useGlobalLights(shader.get());
	scene.useCamera(shader.get());
	/*shader->setUniform("material.ambient", {1.0f, 0.5f, 0.31f,1});
	shader->setUniform("material.diffuse", { 1.0f, 0.5f, 0.31f,1 });
	shader->setUniform("material.specular", {0.5f, 0.5f, 0.5f,1});*/
	shader->setUniform("material.shininess", material.shininess);
	shader->setUniform("material.transparency", material.transparency);
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);

	//shader->setUniform("pointL.position", leftLight.position);
	shader->setUniform("pointL.position", { 1.0f,  1.0f,  0.0f,1 });
	shader->setUniform("pointL.ambient", { 0.05f, 0.05f, 0.05f,1 });
	shader->setUniform("pointL.diffuse", { 0.8f, 0.8f, 0.8f ,1});
	shader->setUniform("pointL.specular", { 1.0f, 1.0f, 1.0f,1 });
	shader->setUniform("pointL.constant", 1.0f);
	shader->setUniform("pointL.linear", 0.09f);
	shader->setUniform("pointL.quadratic", 0.032f);
	mesh->render();
}


void Cube::renderMap(Scene& scene, ppgso::Shader* shader) {
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
}