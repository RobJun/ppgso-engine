#include "Light.h"
#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>

std::unique_ptr<ppgso::Shader> Light::shader;

Light::Light(Scene* scene, const std::string const mesh_filepath,LightConfig config)
{
	color = config.color;
	position = position;
	if (!shader) shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>(mesh_filepath);
}

Light::Light(Scene* scene, const std::string const mesh_filepath)
{
	color = LightConfig().color;
	position = LightConfig().position;
	if (!shader) shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);
	if (!mesh) mesh = std::make_unique<ppgso::Mesh>(mesh_filepath);
}


bool Light::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	return true;
}

void Light::render(Scene& scene)
{

}

void Light::renderMap(Scene& scene, ppgso::Shader* shader) {
	shader->setUniform("ModelMatrix", modelMatrix);
	//shader->setUniform("Texture", *texture);
	mesh->render();
}

void Light::renderLights(Scene& scene)
{
	scene.useCamera(shader.get());
	shader->setUniform("modelMatrix", modelMatrix);
	shader->setUniform("lightColor", color);
	mesh->render();
}
