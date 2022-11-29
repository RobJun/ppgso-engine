#pragma once
#include <memory>
#include <tuple>
#include <ppgso/ppgso.h>

#include "../scene.h"
#include "./object.h"

template<class T>
class ParticleSystem final : public Object {
private:
	float age{ 0 };
public:
	// Speed and rotational momentum
	float speed = 1.f;
	float radius = 0;
	int numberOfParticles = 100;
	glm::vec3 direction = { 0,1,0 };

	float particleLifeSpan = 5.;
	float spawnInterval = 1;


	std::vector<glm::vec3> sizePoints;
	std::vector<float> times;


public:
	ParticleSystem(Scene* scene);

	bool update(Scene& scene, float dt, glm::mat4 parentModelMatrix) override;

	void render(Scene& scene) override;

	void renderMap(Scene& scene, ppgso::Shader* shader) override;

	void renderLights(Scene& scene) override;
};


template <class T>
ParticleSystem<T>::ParticleSystem(Scene* scene){
}

template <class T>
bool ParticleSystem<T>::update(Scene& scene, float dt, glm::mat4 parentModelMatrix)
{
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	age += dt;
	if (age > spawnInterval) {
		for (int i = 0; i < numberOfParticles; i++) {
			std::unique_ptr<Object> particle = std::make_unique<T>(&scene);
			float r = radius * sqrt((rand() % 1000) / 1000.f);
			float t = ((rand() % 1000) / 1000.f) * 2 * 3.14;
			float x = r * cos(t);
			float z = r * sin(t);
			particle->position = { position.x + x,position.y,position.z + z };
			particle->scale = sizeFrames.transformations.size() == 0 ? glm::vec3(1, 1, 1): sizeFrames.transformations[0].transformTo;
			particle->sizeFrames = sizeFrames;
			children.push_back(move(particle));
		}
		age = 0.0;
	}
	auto i = std::begin(children);
	if(sizePoints.size()==0){
		sizePoints.push_back({ 1,1,1 });
		sizePoints.push_back({ 1,1,1 });
	}
	else if (sizePoints.size() == 1) {
		sizePoints.push_back(sizePoints[0]);
	}

	while (i != std::end(children)) {
		auto ch = i->get();
		ch->age += dt;
		float step = (ch->age / particleLifeSpan);
		ch->position += direction * speed * dt + scene.windOnPosition(position);
		ch->update(scene, dt, modelMatrix);
		if (ch->age >= particleLifeSpan)
			i = children.erase(i);
		else
			i++;
	}

	return true;
}
template <class T>
void ParticleSystem<T>::render(Scene& scene)
{
	for (auto& ch : children) {
		ch->render(scene);
	}
}
template <class T>
void ParticleSystem<T>::renderMap(Scene& scene, ppgso::Shader* shader)
{
	for (auto& ch : children) {
		ch->renderMap(scene,shader);
	}
}

template <class T>
void ParticleSystem<T>::renderLights(Scene& scene)
{
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
}