
#include "Plane.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Plane::mesh;
std::unique_ptr<ppgso::Texture> Plane::texture;
std::unique_ptr<ppgso::Shader> Plane::shader;

Plane::Plane(Scene* scene) {
	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/grass2.jpg"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");

	material.diffuse = glm::vec3{ 0.1,	0.35,	0.1 };
	material.ambient = glm::vec3{ 0,	0, 0 };
	material.specular = glm::vec3{ 0.45,	0.55,	0.45 };;
	material.shininess = .25;
	material.transparency = 1;
}


bool Plane::update(Scene& scene, float dt, glm::mat4 parentModelMatrix) {
	rotation = { glm::radians(270.f),0,0 };
	generateModelMatrix();
	normal = glm::normalize(glm::vec3( glm::orientate4(rotation)* glm::vec4(0, 0, 1, 1)));
	for (auto& ch : children) {
		ch->update(scene, dt, glm::mat4(1));
	}
	return true;
}

void Plane::render(Scene& scene) {
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


void Plane::renderMap(Scene& scene, ppgso::Shader* shader) {
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Plane::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}

glm::vec3 Plane::getClosestPoint(glm::vec3 point)
{
	glm::vec3 side = { 1,0,1 };
	side = point * side;
	side.x = side.x > 2*scale.x ? 2 * scale.x : side.x;
	side.z = side.z > 2*scale.z ? 2 * scale.z : side.z;
	return side;
}
