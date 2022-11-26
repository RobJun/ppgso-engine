#include "Bear.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Bear::mesh;
std::unique_ptr<ppgso::Texture> Bear::texture;
std::unique_ptr<ppgso::Shader> Bear::shader;


Bear::Bear(Scene* scene)
{

	material.diffuse = glm::vec3{ 0.714,	0.4284,	0.18144 };
	material.ambient = glm::vec3{ 0.2125,	0.1275,	0.054 };
	material.specular = glm::vec3{ 0.393548,	0.271906,	0.166721 };;
	material.shininess = .2;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/bear.tga"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/bear-obj.obj");
}

bool Bear::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene, dt,modelMatrix);
	}
	return true;
}

void Bear::render(Scene& scene)
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
