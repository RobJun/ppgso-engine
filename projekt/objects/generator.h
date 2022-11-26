#include <glm/glm.hpp>
#include "../objects/object.h"


class Generator final : Object {

public:

	Generator(Scene* scene, Object mesh, int n, int r);
	Generator(Scene* scene, std::vector<Object> mesh) {


	}


	bool update(Scene& scene, float dt, glm::mat4 parentModelMatrix) override;

	void render(Scene& scene) override;

	void renderMap(Scene& scene, ppgso::Shader* shader) override;

	void renderLights(Scene& scene) override;
};