#include "Jaskyna1.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Jaskyna1::mesh;
std::unique_ptr<ppgso::Texture> Jaskyna1::texture;
std::unique_ptr<ppgso::Shader> Jaskyna1::shader;


Jaskyna1::Jaskyna1(Scene* scene)
{
	scale = { .07,.07,.07 };

	material.ambient = glm::vec3{ 0.1,	0.1, 0.1 };
	material.diffuse = glm::vec3{ 0.5,	0.5,	0.0 };
	material.specular = glm::vec3{ 0.6,	0.6,0.5 };;
	material.shininess = .25;
	material.transparency = 1;

	if (!shader) shader = std::make_unique<ppgso::Shader>(our_shader_vert_glsl, our_shader_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadFI("res/stalagnit1.png"));
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>("res/stalagnit1.obj");
}

bool Jaskyna1::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene, dt, modelMatrix);
	}
	return true;
}

void Jaskyna1::render(Scene& scene)
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

void Jaskyna1::renderMap(Scene& scene, ppgso::Shader* shader)
{
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	mesh->render();
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Jaskyna1::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
