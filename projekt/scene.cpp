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
    for (auto& obj : m_objects)
        obj->renderLights(*this);


    shaderSkybox.setUniform("view", glm::mat4(glm::mat3(m_camera->viewMatrix)));
    shaderSkybox.setUniform("projection", m_camera->projectionMatrix);
    skybox.render();
}

void Scene::renderMap(ppgso::Shader* shader) {
	for (auto& obj : m_objects)
		obj->renderMap(*this, shader);
}



void Scene::useGlobalLights(ppgso::Shader* shader) {
    glm::mat4 lightSpaceMatrix = m_globalLight.calculateShadowMap();
    shader->setUniform("lightSpaceMatrix", lightSpaceMatrix);
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


void Scene::useLights(ppgso::Shader* shader) {
    shader->setUniform("number_of_point_lights", number_of_pointLights);
    shader->setUniform("number_of_spot_lights", number_of_spotLights);
    shader->setUniform("num_spot_shadows", number_of_spotLights);
    int i = 0;
    int j = 2;
    for (auto& light : spotLights) {
        if (light->enabled) {
            ppgso::light::Spot* spot = light.get();
            shader->setUniform("lightSpaceMatrix_spot[" + std::to_string(i) + "]",spot->calculateShadowMap());
            shader->setUniform("spotLight[" + std::to_string(i) + "].shadowMap", spot->depthMap, j);
            shader->setUniform("spotLight[" + std::to_string(i) + "].color", spot->color);
            shader->setUniform("spotLight[" + std::to_string(i) + "].position", spot->position);
            shader->setUniform("spotLight[" + std::to_string(i) + "].direction", spot->direction);
            shader->setUniform("spotLight[" + std::to_string(i) + "].ambient", spot->ambient);
            shader->setUniform("spotLight[" + std::to_string(i) + "].diffuse", spot->diffuse);
            shader->setUniform("spotLight[" + std::to_string(i) + "].specular", spot->specular);
            shader->setUniform("spotLight[" + std::to_string(i) + "].constant", spot->constant);
            shader->setUniform("spotLight[" + std::to_string(i) + "].linear", spot->linear);
            shader->setUniform("spotLight[" + std::to_string(i) + "].quadratic", spot->quadratic);
            shader->setUniform("spotLight[" + std::to_string(i) + "].cutOff", glm::cos(spot->cutOff));
            shader->setUniform("spotLight[" + std::to_string(i) + "].outerCutOff", glm::cos(spot->outerCutOff));
            i++;
        }
    }

    i = 0;
    for (auto& light : pointLights) {
        if (light->enabled) {
            ppgso::light::Point* spot = light.get();
            shader->setUniform("pointL[" + std::to_string(i) + "].color", spot->color);
            shader->setUniform("pointL[" + std::to_string(i) + "].position", spot->position);
            shader->setUniform("pointL[" + std::to_string(i) + "].ambient", spot->ambient);
            shader->setUniform("pointL[" + std::to_string(i) + "].diffuse", spot->diffuse);
            shader->setUniform("pointL[" + std::to_string(i) + "].specular", spot->specular);
            shader->setUniform("pointL[" + std::to_string(i) + "].constant", spot->constant);
            shader->setUniform("pointL[" + std::to_string(i) + "].linear", spot->linear);
            shader->setUniform("pointL[" + std::to_string(i) + "].quadratic", spot->quadratic);
            i++;
        }
    }
};