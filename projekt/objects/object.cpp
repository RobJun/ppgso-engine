#include "object.h"

void Object::generateModelMatrix() {
    modelMatrix =
        glm::translate(glm::mat4(1.0f), position)
        * glm::orientate4(rotation)
        * glm::scale(glm::mat4(1.0f), scale);
}
