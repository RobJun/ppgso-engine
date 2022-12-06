#pragma once
#include "shader.h"
#include "depthMap.h"
#include <glm/glm.hpp>
#include <string>

namespace ppgso {
	namespace light {
		enum LightName {
			DIRECT,
			POINT,
			SPOT
		};

		struct Light {
			float ambient;
			float near_plane = 0.01f, far_plane = 100.f;
			float diffuse;
			float specular;
			glm::vec3 color;
			glm::vec3 position;
			//DepthMap depthMap = {1048,1048};
			bool enabled = false;


			virtual glm::mat4 calculateShadowMap() = 0;

		};
		struct Directional : public Light {
			glm::vec3 direction;
			glm::vec2 size = { 30,30 };
			DepthMap depthMap = { 5000,5000 };
			Directional() {};
			Directional(glm::vec3 direction) : direction(direction) {};

			glm::mat4 calculateShadowMap() override {
				glm::mat4 lightProjection = glm::ortho(-size.x, size.x, -size.y, size.y, near_plane, far_plane);
				glm::mat4 lightView = glm::lookAt({-30*direction.x,30*-direction.y,-20*direction.z},//10.f * -direction,
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
			float near_plane = 1.f, far_plane = 30.f;
			DepthMap depthMap = { 1048,1048 };
			glm::mat4 calculateShadowMap() {
				glm::mat4 lightProjection = glm::perspective(glm::radians(90.f),1.0f,near_plane,far_plane);
				auto target = (direction + position);
				//target.y += 0.35;
				glm::mat4 lightView = glm::lookAt(position,
					target,
					glm::vec3(0.0f, 1.0f, 0.0f));
				glm::mat4 lightSpaceMatrix = lightProjection * lightView;
				return lightSpaceMatrix;
			}
		};
	}
}