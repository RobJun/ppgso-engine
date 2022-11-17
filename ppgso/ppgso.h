#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/compatibility.hpp>

#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "loaders/image.h"
#include "loaders/image_bmp.h"
#include "loaders/image_raw.h"
#include "loaders/image_FreeImage.h"
#include "graphics/texture.h"
#include "graphics/window.h"
#include "graphics/material.h"
#include "graphics/lights.h"
#include "graphics/depthMap.h"
#include "graphics/cubemap.h"
namespace ppgso {
  /*!
   * PI constant as defined in glm for float.
   */
  const float PI = glm::pi<float>();
}

