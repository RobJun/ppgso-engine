
#include "Car.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Car::mesh;
std::unique_ptr<ppgso::Texture> Car::texture;
std::unique_ptr<ppgso::Shader> Car::shader;

Car::Car() {
	  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl,diffuse_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/CarTexture.png"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/BasicCar.obj");
}


bool Car::update(Scene& scene, float dt) {
	rotation = rotation + rotMomentum * dt;//glm::vec3(3.14,0,0);
	//scale = { 10,10,10 };
	generateModelMatrix();
    return true;
}

void Car::render(Scene& scene) {
	shader->use();
	// Set up light
	shader->setUniform("directL.direction", glm::vec4(scene.lightDirection,1));
	shader->setUniform("directL.diffuse", glm::vec4(1, 0, 1, 1));
	shader->setUniform("directL.ambient", glm::vec4(0.5, 0.5, 0.5, 1));
	shader->setUniform("directL.specular", glm::vec4(1,1, 1, 1));

	/*shader->setUniform("material.ambient", {1.0f, 0.5f, 0.31f,1});
	shader->setUniform("material.diffuse", { 1.0f, 0.5f, 0.31f,1 });
	shader->setUniform("material.specular", {0.5f, 0.5f, 0.5f,1});
	shader->setUniform("material.shininess", 32.0f);
	shader->setUniform("material.transparency", 1.0f); */
	shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
	shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	shader->setUniform("viewPos", scene.camera.get()->getPosition());
	mesh->render();
}
