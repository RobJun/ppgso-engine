#include "material.h"

namespace ppgso {

	void Material::use(Shader* shader) {
		shader->setUniform("material.diffuse", diffuse);
		shader->setUniform("material.ambient", ambient);
		shader->setUniform("material.specular", specular);
		shader->setUniform("material.shininess", shininess);
		shader->setUniform("material.transparency", transparency);
	}
}