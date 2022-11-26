#include <glm/glm.hpp>
#include <memory>
#include "object.h"
#include "generatorShapes.h"


class Generator final : Object {

public:

	Generator(Scene* scene,Object* mesh, unsigned int n,GeneratorShape shape) {
		for (unsigned int i = 0; i < n; i++) {
			auto position = shape.getRandomPoint();
			//std::unique_ptr<Object> obj = move(mesh->clone());
			//obj->position = this->position + position;
			//children.push_back(move(obj));
		}
	}
	Generator(Scene* scene, std::vector<std::unique_ptr<Object>> meshes,int n,GeneratorShape shape) {
		for (unsigned int i = 0; i < n; i++) {
			auto position = shape.getRandomPoint();
			unsigned int j = rand() % meshes.size();
			//std::unique_ptr<Object> obj = std::make_unique<Object>(*meshes[j]);
			//obj->position = this->position + position;
			//children.push_back(move(obj));
		}
	}


	bool update(Scene& scene, float dt, glm::mat4 parentModelMatrix) override {
		generateModelMatrix();
		modelMatrix = parentModelMatrix * modelMatrix;
		for (auto& ch : children) {
			ch->update(scene,dt,modelMatrix);
		}
		return true;
	}

	void render(Scene& scene) override {

		for (auto& ch : children) {
			ch->render(scene);
		}
	};

	void renderMap(Scene& scene, ppgso::Shader* shader) override {
		for (auto& ch : children) {
			ch->renderMap(scene,shader);
		}
	};

	void renderLights(Scene& scene) override {
		for (auto& ch : children) {
			ch->renderLights(scene);
		}
	};

};