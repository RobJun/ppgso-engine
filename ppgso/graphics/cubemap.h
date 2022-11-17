#pragma once
#include <gl/glew.h>
#include <string>
#include "../loaders/image.h"
#include "../loaders/image_FreeImage.h"
#include "shader.h"

namespace ppgso {

	class CubeMap {
	public:
		GLuint textureID;

		CubeMap(const std::string& directory) {
			const std::string names[] = { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg" };
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
			auto res = directory;
			if (directory[directory.length() - 1] != '/') {
				res += '/';
			}
			for (unsigned int i = 0; i < 6; i++) {
				auto data = image::loadFI(res + names[i]);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, data.width, data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data.getFramebuffer());
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		};

		void bind() {
		};

		void unbind() {

		};
	};


}