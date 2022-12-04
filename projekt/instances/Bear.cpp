#include "Bear.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>

#include "Apple.h"
// Static resources
std::unique_ptr<ppgso::Mesh> Bear::mesh;
std::unique_ptr<ppgso::Texture> Bear::texture;
std::unique_ptr<ppgso::Shader> Bear::shader;


Bear::Bear(Scene* scene)
{

	material.diffuse = glm::vec3{ 0.714,	0.4284,	0.18144 };
	material.ambient = glm::vec3{ 0.01,	0.01,0.01 };
	material.specular = glm::vec3{ 0.393548,	0.271906,	0.166721 };;
	material.shininess = 128;
	material.transparency = 1;

	rotation = { 0,0,3.14 / 2 };

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/bear.tga"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/bear-obj.obj");
}

bool Bear::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	if (glm::distance(position, scene.m_camera->position) > 21) {
		otacajSa(scene,dt);
	}
	else {
		otocSaNaKameru(scene, dt);
	}
	//generateModelMatrix();
	modelMatrix =
		glm::translate(glm::mat4(1.0f), position)
		* glm::orientate4(rotMomentum)
		* glm::translate(glm::mat4(1.0f), actPos)
		* glm::orientate4(glm::vec3{ 0,0,3.14 / 2 })
		* glm::scale(glm::mat4(1.0f), scale);


	actDir = glm::normalize(glm::orientate4(rotMomentum) * glm::orientate4(rotation) * glm::vec4(direction, 1));

	modelMatrix = parentModelMatrix * modelMatrix;

	for (auto& ch : children) {
		ch->update(scene, dt,modelMatrix);
	}
	return true;
}

void Bear::render(Scene& scene)
{
	shader->use();
	//scene.useGlobalLights(shader.get());
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

void Bear::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Bear::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}


void Bear::otacajSa(Scene& s,float dt) {
	float radius = 3;
	float speed = 3;
	rotMomentum += speed * dt* glm::vec3{ 0,0,1 };
}


void Bear::otocSaNaKameru(Scene& s, float dt) {
	glm::vec3 dir = s.m_camera->back;
	float angle = glm::acos(glm::dot(dir, actDir) / (glm::length(dir) * glm::length(actDir)));
	if (glm::distance(actPos, {0,0,0}) > 0.1) {
		float speed = 2;
		actPos -= speed * dt* glm::normalize(actPos);
	}
	if (angle > 0.05) {
		float speed = 2;
		if (rotationDir == 0)
			if (angle > 3.14 / 2)
				rotationDir = 1;
			else
				rotationDir = -1;
		if(rotationDir == -1)
			rotation.z -= dt;
		else 
			rotation.z += dt;
	}
}
