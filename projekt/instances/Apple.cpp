#include "Apple.h"
#include "plane.h"
#include "Rabbit.h"
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
	if (age > 25)
		release = true;
	if (release && !stopMovement) {
		padaj(scene, dt);
		rotation += rotMomentum;
	}
	generateModelMatrix();

	for (auto it = scene.begin(); it != scene.end(); ++it) {
		if (it.get() == this) continue;

		auto zem = dynamic_cast<Plane*>(it.get());

		if (!zem) continue;

		if (!stopMovement){
			if (velocity.y < 0
				&& glm::distance(zem->getClosestPoint(position), position) < 0.2 && position.y > 0) {
				velocity = reflect(velocity);
				bounceCounter++;
				std::cout << velocity.y << " " << position.y << std::endl;
				if (velocity.y < 0.1 || bounceCounter == 5) {
					stopMovement = true;
				}
			}
			if (position.y < zem->position.y) {
				velocity.y *=-0.5 ;
				if (velocity.y < 0.1) {
					stopMovement = true;
				}
			}
		}




	}

	if (stopMovement && !spawnedRabbit) {
		spawnedRabbit = true;
		auto rabbit = std::make_unique<Rabbit>(&scene);
		rabbit->scale = { 0.2,0.2,0.2 };
		rabbit->position = { -10,0.5,25 };
		rabbit->rotation = { 0,0,2 };


		//rabbit translation
		ppgso::KeyFrame<glm::vec3> firstPosition;
		firstPosition.transformTo = { -10,0.3,25 };
		firstPosition.time = 0;
		rabbit->translateFrames.addFrame(firstPosition);

		ppgso::KeyFrame<glm::vec3> finalPosition;
		finalPosition.transformTo = position + glm::vec3{-1,0.3,1};
		finalPosition.time = 5;
		finalPosition.interpolation = ppgso::LINEAR;
		rabbit->translateFrames.addFrame(finalPosition);

		finalPosition.transformTo = position + glm::vec3{ -1,0.3,1 };
		finalPosition.time = 20;
		finalPosition.interpolation = ppgso::LINEAR;
		rabbit->translateFrames.addFrame(finalPosition);

		finalPosition.transformTo = { -20,0.3,25 };
		finalPosition.time = 23;
		finalPosition.interpolation = ppgso::LINEAR;
		rabbit->translateFrames.addFrame(finalPosition);

		//rabbit rotation

		finalPosition.transformTo = glm::vec3{ 0,0,2 };
		finalPosition.time = 17;
		finalPosition.interpolation = ppgso::CONSTANT;
		rabbit->rotationFrames.addFrame(finalPosition);

		finalPosition.transformTo = { 0,00,2+3.14 };
		finalPosition.time = 20;
		finalPosition.interpolation = ppgso::LINEAR;
		rabbit->rotationFrames.addFrame(finalPosition);


		children.push_back(move(rabbit));



	}

	modelMatrix = parentModelMatrix * modelMatrix;

	for (auto i = children.begin(); i != children.end();) {
		auto obj = i->get();
		if (!obj->update(scene, dt, modelMatrix))
			i = children.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
		else
			++i;
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
	velocity += gravityAcc * dt; + scene.windOnPosition(position);
	rotMomentum.y += 0.01 * dt;

}

glm::vec3 Apple::reflect(glm::vec3 velocity) {
	auto vel = velocity;
	float u = 0.05;
	float e = 0.1;
	float radius = scale.x / 2;
	float i = 2;
	vel.y *= -0.7;
	vel.x = (velocity.x - u * (1 + e) * velocity.y);
	vel.z = (velocity.z + u * (1 + e) * velocity.y);
	rotMomentum.y -= u * (1 + e) * velocity.y*(radius/i);
	return vel;

}
