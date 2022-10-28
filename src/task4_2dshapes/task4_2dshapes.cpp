// Task 5 - Draw a 2D shape using polygons and animate it
//        - Encapsulate the shape using a class
//        - Use color_vert/frag shader to display the polygon
//        - Animate the object position, rotation and scale.

#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 512;


glm::mat4 basic(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::mat4 parent) {
    return  glm::translate(glm::mat4(), position) * glm::rotate(glm::mat4(), glm::radians(rotation.z), { 0,0,1 }) * parent;
}
glm::mat4 roatateAround(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::mat4 parent) {
    return parent* glm::rotate(glm::mat4(), glm::radians(rotation.z), { 0,0,1 }) * glm::translate(glm::mat4(), position) * glm::rotate(glm::mat4(), glm::radians(90.f), { 0,0,1 });
}
// Object to represent 2D OpenGL shape
class Shape {
private:
  // 2D vectors define points/vertices of the shape
  // TODO: Define your shape points
    std::vector<glm::vec3> vetrices =/// { {0,0.5f,0},{0.5f,-0.5f,0},{0,-0.5f,0}, {-0.5f,-0.5f,0},{0.25f,-0.75f,0},{-0.25f,-0.75f,0} };
    { {-0.2f,0.1f,0},
      {0.2f,0.1f,0},
      {0.2,-0.1f,0},
      {-0.2f,-0.1f,0},
      {0.3f,0,0},
      {-0.25,-0.2,0},
      {-0.15,-0.2,0},
      {0.25,-0.2,0},
      {0.15,-0.2,0},

    };
  // Structure representing a triangular face, usually indexes into vertices
  struct Face {
    // TODO: Define your face structure
      GLint p1;
      GLint p2;
      GLint p3;
  };

  // Indices define triangles that index into vertices
  // TODO: Define your mesh indices
  std::vector<Face> mesh = { {0,1,2},{2,3,0},{1,2,4},{3,5,6},{2,7,8} };

  // Program to associate with the object
  ppgso::Shader program = {color_vert_glsl, color_frag_glsl};

  // These will hold the data and object buffers
  GLuint vao, vbo, cbo, ibo;
  glm::mat4 modelMatrix{1.0f};
public:
  // Public attributes that define position, color ..
  glm::vec3 position{0,0,0};
  glm::vec3 rotation{0,0,0};
  glm::vec3 scale{1,1,1};
  glm::vec3 color{1,0,0};
  glm::mat4 (*modify)(glm::vec3,glm::vec3,glm::vec3,glm::mat4);
  Shape* parent = nullptr;

  // Initialize object data buffers
  Shape() {
    // Copy data to OpenGL
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Copy positions to gpu
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vetrices.size() * sizeof(glm::vec3), vetrices.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto position_attrib = program.getAttribLocation("Position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Copy mesh indices to gpu
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size() * sizeof(Face), mesh.data(), GL_STATIC_DRAW);

    // Set projection matrices to identity
    program.setUniform("ProjectionMatrix", glm::mat4{1.0f});
    program.setUniform("ViewMatrix", glm::mat4{1.0f});
  };
  // Clean up
  ~Shape() {
    // Delete data from OpenGL
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &cbo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }

  // Set the object transformation matrix
  void update() {
      //modelMatrix = translate(position) * scaleM(scale) * rotate(glm::radians(rotation.z));
      modelMatrix = modify( position,scale,rotation,parent == nullptr? glm::mat4() : parent->modelMatrix);

  }

  // Draw polygons
  void render(){
    // Update transformation and color uniforms in the shader
    program.use();
    program.setUniform("OverallColor", color);
    program.setUniform("ModelMatrix", modelMatrix);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh.size() * 3, GL_UNSIGNED_INT, 0);
  };
};

class ShapeWindow : public ppgso::Window {
private:
  Shape shape1, shape2,shape3;
public:
  ShapeWindow() : Window{"task4_2dshapes", SIZE, SIZE} {
    shape1.color = {1,0,0};
    shape2.color = {0,1,0};
    shape3.color = { 0,0,1 };
    shape1.modify = &basic;
    shape2.modify = &basic;
    shape3.modify = &roatateAround;
    shape3.parent = &shape1;
  }

  void onIdle() {
    // Set gray background
    glClearColor(.1f,.1f,.1f,1.0f);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Move and Render shape\    // Get time for animation
    auto t = (float) glfwGetTime();

    // TODO: manipuate shape1 and shape2 position to rotate clockwise
    float jump = ((int)t * 5) % 180;
    shape1.position = { (t-1)/5,sin(t)/2 ,0 };
    shape2.position = { (t-0.5)/10,sin(t)/2 ,0 };


    shape3.rotation.z = 20.f * t;
    shape3.position = { 0.5f,0,0 };

    // Manipulate rotation of the shape
    //shape1.rotation.z = t*5.0f;
    //shape2.rotation = -shape1.rotation;

    // Manipulate shape size
    //shape1.scale = {sin(t),sin(t), 1};
    //shape2.scale = -shape1.scale;

    // Update and render each shape
    shape1.update();
    shape2.update();
    shape3.update();

    shape1.render();
    shape2.render();
    shape3.render();
  }
};

int main() {
  // Create our window
  auto window = ShapeWindow{};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
