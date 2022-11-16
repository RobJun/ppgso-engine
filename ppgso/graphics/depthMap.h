#pragma once
#include <gl/glew.h>

namespace ppgso {
	class DepthMap {
	private:
		
		unsigned int width,height;
		GLuint i_depthMap,fbo_depthMap;
		
	public:
		DepthMap(unsigned int width, unsigned int height) : width(width),height(height) {
			glGenFramebuffers(1, &fbo_depthMap);
			glGenTextures(1, &i_depthMap);
			glBindTexture(GL_TEXTURE_2D, i_depthMap);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_depthMap);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, i_depthMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		~DepthMap() {
			glDeleteTextures(1, &i_depthMap);
		}
		unsigned int getWidth() { return width; };
		unsigned int getHeight() { return height; };
		void bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_depthMap);
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		GLuint getTexture() { return i_depthMap; };
		GLuint getFBO() { return fbo_depthMap; };

		void unbind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void bindTexture(int id) const {
			glActiveTexture((GLenum)(GL_TEXTURE0 + id));
			glBindTexture(GL_TEXTURE_2D, i_depthMap);
		}
	};


}