// Task 7 - Dynamically generate objects in a 3D scene
//        - Implement a particle system where particles have position and speed
//        - Any object can be a generator and can add objects to the scene
//        - Create dynamic effect such as fireworks, rain etc.
//        - Encapsulate camera in a class

#include <iostream>
#include <vector>
#include <map>
#include <list>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 1024;

class Camera {
public:
  // TODO: Add parameters
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;

  /// Representaiton of
  /// \param fov - Field of view (in degrees)
  /// \param ratio - Viewport ratio (width/height)
  /// \param near - Distance of the near clipping plane
  /// \param far - Distance of the far clipping plane
  Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 100.0f) {
      float fowInRad = (ppgso::PI / 180.0f) * fov;

      projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
  }

  /// Recalculate viewMatrix from position, rotation and scale
  void update() {
      viewMatrix = lookAt(glm::vec3(0, 0, 0), glm::vec3(0,0,1) ,glm::vec3(0,1,0));
  }
};

/// Abstract renderable object interface
class Renderable; // Forward declaration for Scene
using Scene = std::vector<std::unique_ptr<Renderable>>; // Type alias

class Renderable {
public:
  // Virtual destructor is needed for abstract interfaces
  virtual ~Renderable() = default;

  /// Render the object
  /// \param camera - Camera to use for rendering
  virtual void render(const Camera& camera) = 0;

  /// Update the object. Useful for specifing animation and behaviour.
  /// \param dTime - Time delta
  /// \param scene - Scene reference
  /// \return - Return true to keep object in scene
  virtual bool update(float dTime, Scene &scene) = 0;
  virtual glm::vec3 getPosition() {
      return glm::vec3();
  };
};

/// Basic particle that will render a sphere
/// TODO: Implement Renderable particle
class Sun final : public Renderable {
  // Static resources shared between all particles
  static std::unique_ptr<ppgso::Mesh> mesh;
  static std::unique_ptr<ppgso::Shader> shader;

  // TODO: add more parameters as needed
  glm::vec3 p, s, c;
  glm::mat4 ModelMatrix;
  float age = 0.0f;
  glm::vec3 rotation = { 0,0,1 };
  glm::vec3 scale = { 3,3,3 };
public:
  /// Construct a new Particle
  /// \param p - Initial position
  /// \param s - Initial speed
  /// \param c - Color of particle
  Sun(glm::vec3 p, glm::vec3 s, glm::vec3 c) 
               : p(p),s(s),c(c){
    // First particle will initialize resources
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
  }

  bool update(float dTime, Scene &scene) override {
      age += dTime;
      //if (age > 10.f) return false;
      rotation += s;
      //p += s;//*dTime;
      ModelMatrix = glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0,1,0)) *glm::translate(glm::mat4(1.0f), p)
          //* glm::orientate4(rotation)
          * glm::scale(glm::mat4(1.0f), scale);
      return true;

  }

  void render(const Camera& camera) override {

      shader->use();
      shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
      shader->setUniform("ViewMatrix", camera.viewMatrix);
      shader->setUniform("OverallColor", c);
      shader->setUniform("ModelMatrix", ModelMatrix);
      mesh->render();
  }

  glm::vec3 getPosition() {
      return  glm::vec3( ModelMatrix* glm::vec4(p, 1));
  };
};

class Sun_flare final : public Renderable {
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    // TODO: add more parameters as needed
    glm::mat4 ModelMatrix;
    float age = 0.0f;
    glm::vec3 rotation = { 0,0,1 };
    glm::vec3 scale = { 0.5,1,0.5 };
    glm::vec3 p, s, c;
public:
    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Sun_flare(glm::vec3 p, glm::vec3 s, glm::vec3 c)
        : p(p), s(s), c(c) {
        // First particle will initialize resources
        if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
    }

    bool update(float dTime, Scene& scene) override {
        age += dTime;
        if (age > 1.f)
            return false;
        p += s;//*dTime;
        scale = s * 10.f;
        //s = -1.f * s;
        ModelMatrix = glm::translate(glm::mat4(1.0f), p)
            * glm::scale(glm::mat4(1.0f), scale);

        return true;

    }

    void render(const Camera& camera) override {

        shader->use();
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);
        shader->setUniform("OverallColor", c);
        shader->setUniform("ModelMatrix", ModelMatrix);
        mesh->render();
    }

    glm::vec3 getPosition()  {
        return  glm::vec3((glm::vec4(1, 1, 1, 0)*ModelMatrix));
    }
};


class Moon_flare final : public Renderable {
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    // TODO: add more parameters as needed
    glm::mat4 ModelMatrix;
    float age = 0.0f;
    glm::vec3 rotation = { 0,0,1 };
    glm::vec3 scale = { 0.5,5,0.5 };
    glm::vec3 p, s, c;
public:
    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Moon_flare(glm::vec3 p, glm::vec3 s, glm::vec3 c)
        : p(p), s(s), c(c) {
        // First particle will initialize resources
        if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
    }

    bool update(float dTime, Scene& scene) override {
        age += dTime;
        if (age > 3.f)
            return false;
        p += s;//*dTime;
        s = -1.f * s;
        //rotation = s * 0.2f;
        ModelMatrix =glm::translate(glm::mat4(1.0f), p)
            * glm::scale(glm::mat4(1.0f), scale);

        return true;

    }

    void render(const Camera& camera) override {

        shader->use();
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);
        shader->setUniform("OverallColor", c);
        shader->setUniform("ModelMatrix", ModelMatrix);
        mesh->render();
    }

    glm::vec3 getPosition() {
        return  glm::vec3((glm::vec4(1, 1, 1, 0) * ModelMatrix));
    }
};


class Earth final : public Renderable {
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    // TODO: add more parameters as needed
    glm::vec3 p, s, c;
    glm::mat4 ModelMatrix;
    float age = 0.0f;
    glm::vec3 rotation = { 1,1,1 };
    glm::vec3 scale = { 10,8,8 };
public:
    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Earth(glm::vec3 p, glm::vec3 s, glm::vec3 c)
        : p(p), s(s), c(c) {
        // First particle will initialize resources
        if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
    }

    bool update(float dTime, Scene& scene) override {
        age += dTime;
        //if (age > 10.f) return false;
        p += s;//*dTime;
        ModelMatrix = glm::translate(glm::mat4(1.0f), p)
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);
        return true;

    }

    void render(const Camera& camera) override {

        shader->use();
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);
        shader->setUniform("OverallColor", c);
        shader->setUniform("ModelMatrix", ModelMatrix);
        mesh->render();
    }

    glm::vec3 getPosition() {
        return p;
    }
};
// Static resources need to be instantiated outside of the class as they are globals
std::unique_ptr<ppgso::Mesh> Sun::mesh;
std::unique_ptr<ppgso::Shader> Sun::shader;
std::unique_ptr<ppgso::Mesh> Earth::mesh;
std::unique_ptr<ppgso::Shader> Earth::shader;
std::unique_ptr<ppgso::Mesh> Sun_flare::mesh;
std::unique_ptr<ppgso::Shader> Sun_flare::shader;
std::unique_ptr<ppgso::Mesh> Moon_flare::mesh;
std::unique_ptr<ppgso::Shader> Moon_flare::shader;

class ParticleWindow : public ppgso::Window {
private:
  // Scene of objects
  Scene scene;

  // Create camera
  Camera camera = {120.0f, (float)width/(float)height, 1.0f, 400.0f};

  // Store keyboard state
  std::map<int, int> keys;
public:
  ParticleWindow() : Window{"task7_particles", SIZE, SIZE} {
    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    scene.push_back(std::make_unique<Sun>(glm::vec3(0,0,10), glm::vec3( 0,0,0.001 ), glm::vec3( 1,0.7,0.2 )));
    scene.push_back(std::make_unique<Sun>(glm::vec3(0, 0, -10), glm::vec3(0, 0, 0.001), glm::vec3(0.3, 0.3, 0.3)));
    scene.push_back(std::make_unique<Earth>(glm::vec3(0, -6, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

  }

  void onKey(int key, int scanCode, int action, int mods) override {
    // Collect key state in a map
    keys[key] = action;
    if (keys[GLFW_KEY_SPACE]) {
      // TODO: Add renderable object to the scene
        glm::vec3 sun_position = scene.front()->getPosition();
        glm::vec3 earth_position = { 0,-6,0 };
        auto smer = earth_position- sun_position;
        scene.push_back(std::make_unique<Sun_flare>(
            sun_position, smer * 0.001f, glm::vec3(1, 0.7, 0.2)
           ));

         glm::vec3 moon_position = scene[2]->getPosition();
         smer = earth_position - moon_position;
        scene.push_back(std::make_unique<Sun_flare>(
            moon_position, smer * 0.001f, glm::vec3(1, 1, 1)
            ));

    }
  }

  void onIdle() override {
    // Track time
    static auto time = (float) glfwGetTime();
    // Compute time delta
    float dTime = (float)glfwGetTime() - time;
    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.1f,.1f,.1f,1.0f);

    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.update();

    // Update all objects in scene
    // Because we need to delete while iterating this is implemented using c++ iterators
    // In most languages mutating the container during iteration is undefined behaviour
    auto i = std::begin(scene);
    while (i != std::end(scene)) {
        // Update object and remove from list if needed
        auto obj = i->get();
        if (!obj->update(dTime, scene))
            i = scene.erase(i);
        else
            ++i;
    }

    // Render every object in scene
    for(auto& object : scene) {
      object->render(camera);
    }
  }
};

int main() {
  // Create new window
  auto window = ParticleWindow{};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
