#include <glm/glm.hpp>
#include <memory>
#include "object.h"
#include "generatorShapes.h"
#include "../instances/Tree1.h"
#include "../instances/Tree3.h"


void tranformTrees(Object* obj) {
	float random_scale = 0.4 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.9 - 0.4)));
	obj->scale = { random_scale, random_scale, random_scale };
}

template <class T, class U>
class Generator final : public Object {

public:


	Generator(Scene* scene, unsigned int n, GeneratorShape* shape, void (*transform)(Object*) = nullptr);


	bool update(Scene& scene, float dt, glm::mat4 parentModelMatrix) override;

	void render(Scene& scene) override;
	void renderMap(Scene& scene, ppgso::Shader* shader) override;

	void renderLights(Scene& scene) override;

};


template <class T,class U>
Generator<T,U>::Generator(Scene* scene, unsigned int n, GeneratorShape* shape, void (*transform)(Object*)) {
	srand(time(NULL));
	for (unsigned int i = 0; i < n; i++) {
		auto position = shape->getRandomPoint();
		std::unique_ptr<Object> obj;
		if(rand() %2 )
			obj = std::make_unique<T>(scene);
		else
			obj = std::make_unique<U>(scene);
		
		if (transform != nullptr)
			transform(obj.get());
	
		obj->position = this->position + position;
		children.push_back(move(obj));
	}
}

template <class T, class U>
bool Generator<T,U>::update(Scene& scene, float dt, glm::mat4 parentModelMatrix) {
	generateModelMatrix();
	modelMatrix = parentModelMatrix * modelMatrix;
	for (auto& ch : children) {
		ch->update(scene, dt, modelMatrix);
	}
	return true;
}

template <class T, class U>
void  Generator<T,U>::render(Scene& scene) {

	for (auto& ch : children) {
		ch->render(scene);
	}
};

template <class T, class U>
void  Generator<T,U>::renderMap(Scene& scene, ppgso::Shader* shader) {
	for (auto& ch : children) {
		ch->renderMap(scene, shader);
	}
};

template <class T, class U>
void Generator<T,U>::renderLights(Scene& scene) {
	for (auto& ch : children) {
		ch->renderLights(scene);
	}
};