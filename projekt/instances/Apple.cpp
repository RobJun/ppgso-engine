#include "Apple.h"
#include "plane.h"
#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Apple::mesh;
std::unique_ptr<ppgso::Texture> Apple::texture;
std::unique_ptr<ppgso::Shader> Apple::shader;


Apple::Apple(Scene* scene)
{
	material.ambient = glm::vec3{ 0.05,	0.05,0.0 };
	material.diffuse = glm::vec3{ 0.5,0.5,0.4 };
	material.specular = glm::vec3{ 0.7,	0.7,0.04 };
	material.shininess = .078125;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/apple.jpg"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/apple.obj");
}

bool Apple::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	age += dt;
	if(!stopMovement)
		padaj(scene,dt);
	generateModelMatrix();

	for (auto it = scene.begin(); it != scene.end(); ++it) {
		if (it.get() == this) continue;

		auto zem = dynamic_cast<Plane*>(it.get());

		if (!zem) continue;

		if (velocity.y < 0 && velocity.y > -0.1) {
			std::cout << "hereeeee " << velocity.y << " " << std::endl;
			velocity = { 0,0,0 };
			stopMovement = true;
		}

		//std::cout << glm::distance(position, zem->getClosestPoint(position)) << std::endl;
		if (velocity.y < 0 && position.y < 0) {
			bounced = true;
			velocity.y *= -0.5;
			std::cout << "here " <<velocity.y<< " " << std::endl;
			/*if (abs(velocity.y) < 0.1) {
				std::cout << "hereeeee " << velocity.y << " " << std::endl;
				velocity = { 0,0,0 };
				stopMovement = true;
			}*/
		}




	}

	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene, dt, modelMatrix);
	}
	return true;
}

void Apple::render(Scene& scene)
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

void Apple::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Apple::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}


void Apple::padaj(Scene& scene,float dt) {
	glm::vec3 windV = scene.windOnPosition(position);
	glm::vec3 gravityAcc = { 0,-scene.g,0 };
	
	position += dt * (velocity + dt * gravityAcc / 2.f);
	velocity += gravityAcc * dt + scene.windOnPosition(position);

}
