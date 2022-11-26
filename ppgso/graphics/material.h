#pragma once
#include <glm/glm.hpp>
#include "shader.h"

namespace ppgso {

	struct Material {
		glm::vec3 diffuse;
		glm::vec3 ambient;
		glm::vec3 specular;

		float shininess;
		float transparency;

		void use(Shader* shader);
	};
}