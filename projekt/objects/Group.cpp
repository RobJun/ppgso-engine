#include "Group.h"

#include <shaders/our_shader_vert_glsl.h>
#include <shaders/our_shader_frag_glsl.h>


Group::Group(Scene* scene) {
	translation = { 5,0,0 };
}

bool Group::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{

	position += dt * translation;

	generateModelMatrix();
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

void Group::render(Scene& scene)
{
	for (auto& ch : children) {
		ch->render(scene);
	}
}

void Group::renderMap(Scene& scene, ppgso::Shader* shader)
{
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
}

void Group::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}
