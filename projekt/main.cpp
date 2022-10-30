#include <ppgso/ppgso.h>
#include <memory>

#include <shaders/renderDepthMap_vert_glsl.h>
#include <shaders/renderDepthMap_frag_glsl.h>

#include <shaders/deb_vert_glsl.h>
#include <shaders/deb_frag_glsl.h>

#include "scene.h"
#include "instances/plane.h"
#include "instances/car.h"
#include "instances/cube.h"

#define SIZE  512


#define DEBUG_SHADOW_MAPS 0

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

class OurWindow : public ppgso::Window {
private:
    Scene* m_scene;;
    ppgso::Shader shader = { renderdepthmap_vert_glsl , renderdepthmap_frag_glsl};
    ppgso::Shader shader_debug = { deb_vert_glsl , deb_frag_glsl };
public:
    OurWindow() : Window{ "projekt", SIZE, SIZE } { initWindow(); };
    OurWindow(const int& size, const double& const aspectRatio) : Window{ "projekt", size, (int)(size * aspectRatio) } { initWindow(); };

public: 
    Scene* switchScene(Scene* scene) {
        Scene* temp = m_scene;
        m_scene = scene;
        return temp;
    }
private: 

    //returns scene for deletion
    void initWindow() {
        //hideCursor();
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);


        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader_debug.use();
        shader_debug.setUniform("depthMap", 0);
        //initScene();

    }

    void onIdle() override {
        // Track time
        static auto time = (float)glfwGetTime();

        // Compute time delta
        float dt = (float)glfwGetTime() - time;

        time = (float)glfwGetTime();
        m_scene->update(dt);
        //generate shadowMap for direct light
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 lightSpaceMatrix = m_scene->m_globalLight.calculateShadowMap();
        shader.use();
        shader.setUniform("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, m_scene->m_globalLight.depthMap.getWidth(), m_scene->m_globalLight.depthMap.getHeight());
        m_scene->m_globalLight.depthMap.bind();
        m_scene->renderMap(&shader);
        m_scene->m_globalLight.depthMap.unbind();

        //generate shadowMap for point lights
        //generate shadowMap for spot lights
        for (auto& light : m_scene->spotLights) {
            if (light->enabled) {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glm::mat4 lightSpaceMatrix = light->calculateShadowMap();
                shader.use();
                shader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
                glViewport(0, 0, light->depthMap.getWidth(), light->depthMap.getHeight());
                light->depthMap.bind();
                m_scene->renderMap(&shader);
                light->depthMap.unbind(); 
            }
        }
#if DEBUG_SHADOW_MAPS
        shader_debug.use();
        shader_debug.setUniform("near_plane", m_scene->spotLights[0]->near_plane);
        shader_debug.setUniform("far_plane", m_scene->spotLights[0]->far_plane);
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, m_scene->m_globalLight.depthMap.getTexture());
        glBindTexture(GL_TEXTURE_2D, m_scene->spotLights[1]->depthMap.getTexture());
        renderQuad();
#else    
        glViewport(0, 0, width, height);
        glClearColor(1.f, 1.f, 1.f, 1.f);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_scene->render();
#endif
    }
};


Scene* createScene1() {
    Scene* scene = new Scene;
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 0.2f;
    scene->m_globalLight.diffuse = 0.3f ;
    scene->m_globalLight.specular= 0.8f;
    scene->m_globalLight.color = {1,1,1};

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = -40.0f;
    scene->m_camera = move(camera);
    auto car = std::make_unique<Car>(scene);
    car->position = { 0,0.75,0 };
    car->rotation = { 0,0,3.14f / 2 };
    scene->m_objects.push_back(move(car));
    //auto car = std::make_unique<Car>(scene);
    //car->position = { 0,0,5 };
    //scene->m_objects.push_back(move(car));
    car = std::make_unique<Car>(scene);
    car->position = { 0,0,-10 };
    car->rotMomentum = { 0,0,0 };
    scene->m_objects.push_back(move(car));
    scene->m_objects.push_back(std::make_unique<Plane>(scene));

    auto cube = std::make_unique<Cube>();
    cube->position = { 0,0,10 };
    cube->scale = { 7,7,7 };
    scene->m_objects.push_back(move(cube));
    return scene;
}


int main() {
    // Initialize our window
    OurWindow window = {1200,16/9 };
    Scene* scene = createScene1();
    window.switchScene(scene);
    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}