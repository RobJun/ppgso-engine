#pragma once
#include "shader.h"
#include "depthMap.h"
#include <glm/glm.hpp>
#include <string>

namespace ppgso {
	namespace light {
		struct Light {
			float ambient;
			float near_plane = 0.1f, far_plane = 75.f;
			float diffuse;
			float specular;
			glm::vec3 color;
			glm::vec3 position;
			DepthMap depthMap = {2048,2048};
			bool enabled = false;


			virtual glm::mat4 calculateShadowMap() = 0;

		};
		struct Directional : public Light {
			glm::vec3 direction;
			Directional() {};
			Directional(glm::vec3 direction) : direction(direction) {};

			glm::mat4 calculateShadowMap() override {
				glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
				glm::mat4 lightView = glm::lookAt(20.f*-direction,
												glm::vec3(0.0f,0.0f,0.0f),
												  glm::vec3(0.0f, 1.0f, 0.0f));
				glm::mat4 lightSpaceMatrix = lightProjection * lightView;
				return lightSpaceMatrix;
			}
		};

		struct Point : public Light {
			float constant;
			float linear;
			float quadratic;

			glm::mat4 calculateShadowMap() {
				return glm::mat4(1);
			}
		};

		struct Spot : public Light {
			glm::vec3  direction;
			float cutOff;
			float constant;
			float linear;
			float quadratic;
			float outerCutOff;

			glm::mat4 calculateShadowMap() {
				glm::mat4 lightProjection = glm::perspective(glm::radians(90.f),1.0f,near_plane,far_plane);
				glm::mat4 lightView = glm::lookAt(position,
					direction+position,
					glm::vec3(0.0f, 1.0f, 0.0f));
				glm::mat4 lightSpaceMatrix = lightProjection * lightView;
				return lightSpaceMatrix;
			}
		};
	}
}