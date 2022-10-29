#include "scene.h"



void Scene::update(float time) {
	m_camera->update(time);

	for (auto i = m_objects.begin(); i != m_objects.end();) {
		auto obj = i->get();
		if (!obj->update(*this,time))
			i = m_objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
		else
			++i;
	}
}


void Scene::render() {
	//m_globalLight.direction += glm::vec4(0, 0,, 0);
	//std::cout << m_globalLight.direction.z << std::endl;
	for (auto& obj : m_objects)
		obj->render(*this);
}

void Scene::renderMap(ppgso::Shader* shader) {
	for (auto& obj : m_objects)
		obj->renderMap(*this, shader);
}



void Scene::useGlobalLights(ppgso::Shader* shader) {
	shader->setUniform("directL.shadowMap", m_globalLight.depthMap);
	shader->setUniform("directL.color", m_globalLight.color);
	shader->setUniform("directL.direction", m_globalLight.direction);
	shader->setUniform("directL.diffuse", m_globalLight.diffuse);
	shader->setUniform("directL.ambient", m_globalLight.ambient);
	shader->setUniform("directL.specular", m_globalLight.specular);
}

void Scene::useCamera(ppgso::Shader* shader) {
	shader->setUniform("ProjectionMatrix", m_camera->projectionMatrix);
	shader->setUniform("ViewMatrix", m_camera->viewMatrix);
	shader->setUniform("viewPos", m_camera.get()->getPosition());
}