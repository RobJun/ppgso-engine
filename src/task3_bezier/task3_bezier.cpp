// Task 4 - Render the letter R using OpenGL
//        - Implement a function to generate bezier curve points
//        - Generate multiple points on each bezier curve
//        - Draw lines connecting the generated points using OpenGL

#include <iostream>
#include <vector>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 512;

class BezierWindow : public ppgso::Window {
private:

  // Control points for the bezier curves
  // First curve is 4 control points
  // Rest of the curves are 3 control points, each reusing the last curve end
  // Defines the letter "R"
  std::vector<glm::vec2> controlPoints = {
      {0,0},
      {1,0.5},
      {0.0,1},
      {0.0,0.5},
      {0.0,1},
      {-1,0.5},
      {0,0},
  };

  // This will hold the bezier curve geometry once we generate it
  std::vector<glm::vec3> points;

  // GLSL Program to use for rendering
  ppgso::Shader program = {color_vert_glsl, color_frag_glsl};

  // These numbers are used to pass buffer data to OpenGL
  GLuint vao = 0, vbo = 0;

  // Compute points for Bezier curve using 4 control points
  glm::vec2 bezierPoint(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3, const float t) {
    // TODO: Compute point on the Bezier curve
      glm::vec2 p11 = (1 - t) * p0 + t * p1, p21 = (1 - t) * p1 + t * p2, p31= (1 - t) * p2 + t * p3;
      glm::vec2 p22 = (1 - t) * p11 + t * p21 , p32 = (1 - t) * p21 + t * p31;
      return (1-t)*p22+t*p32;
  }
  std::vector<glm::vec3> rotate(std::vector<glm::vec3> points, const float degree) {
      std::vector<glm::vec3> result;
      glm::mat3x3 rotation = { cos(degree),-sin(degree),0,sin(degree),cos(degree),0,0,0,1 };
      for (auto point : points) {
          result.push_back(point * rotation);
      }
      return result;
  }

  glm::vec2 bezierRecursicve(std::vector<glm::vec2> points,int j,const float t, const int n){
      //std::cout << "point " << j << n << std::endl;
      if (n == 0) return points[j];
      auto P1 = bezierRecursicve(points, j-1, t, n-1);
      auto P2 = bezierRecursicve(points, j, t, n-1);
      return (1 - t) * P1 + t * P2;
  }

  // Compute points for a sequence of Bezier curves defined by a vector of control points
  // Each bezier curve will reuse the end point of the previous curve
  // count - Number of points to generate on each curve
  void bezierShape(int count) {
      std::cout << controlPoints.size();
      int n = 6;
    for(int i = 1; i < (int) controlPoints.size(); i+=n) {
      for (int j = 0; j <= count; j++) {
        // TODO: Generate points for each Bezier curve and insert them
       // std::cout << bezierPoint(controlPoints[i-1], controlPoints[i], controlPoints[i + 1], controlPoints[i + 2],((float)j)/count) << std::endl;
        glm::vec2 point = bezierRecursicve(controlPoints, n+i-1, ((float)j) / count, n);
        points.emplace_back(point, 0);
      }
    }
  }

public:
  BezierWindow() : Window{"task3_bezier", SIZE, SIZE} {
    // Generate Bezier curve points
    bezierShape(100);
    /*auto f = rotate(points, 3.14 / 2);
    auto f2 = rotate(points, 3.14);
    auto f3 = rotate(points, -3.14/2);
    points.insert(points.end(), f.begin(), f.end());
    points.insert(points.end(), f2.begin(), f2.end());
    points.insert(points.end(), f3.begin(), f3.end());*/

    // Generate a vertex array object
    // This keeps track of what attributes are associated with buffers
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a vertex buffer object, this will feed data to the vertex shader
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // TODO: Pass the control points to the GPU
    // glBufferData(GL_ARRAY_BUFFER, ???, ???, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(glm::vec3),&points[0], GL_STATIC_DRAW);

    // Setup vertex array lookup, this tells the shader how to pick data for the "Position" input
    auto position_attrib = program.getAttribLocation("Position");
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(position_attrib);

    // Set model matrix to identity
    program.setUniform("ModelMatrix", glm::mat4{});
    program.setUniform("ViewMatrix", glm::mat4{});
    program.setUniform("ProjectionMatrix", glm::mat4{});

    // Set the color uniform
    program.setUniform("OverallColor", glm::vec3{1.0f,1.0f,1.0f});
  }

  ~BezierWindow() final {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }

  void onIdle() final {
    // Set gray background
    glClearColor(0.5f,0.5f,0.5f,0);

    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw shape
    glBindVertexArray(vao);

    // TODO: Define the correct render mode
    //glDrawArrays(??, 0, ??);
    glDrawArrays(GL_LINE_LOOP, 0, points.size()+1);
  }
};

int main() {
  // Create our window
  BezierWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
