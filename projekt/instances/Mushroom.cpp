#include "Mushroom.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Mushroom::mesh;
std::unique_ptr<ppgso::Texture> Mushroom::texture;
std::unique_ptr<ppgso::Shader> Mushroom::shader;


Mushroom::Mushroom(Scene* scene)
{
	material.ambient = glm::vec3{ 0.0,	0.0, 0.0 };
	material.diffuse = glm::vec3{ 0.5,0.0,0.0 };
	material.specular = glm::vec3{ 0.7,	0.6,0.6 };;
	material.shininess = .25;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/Mushroom.png"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/Mushroom.obj");
}

bool Mushroom::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene, dt, modelMatrix);
	}
	return true;
}

void Mushroom::render(Scene& scene)
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

void Mushroom::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Mushroom::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
