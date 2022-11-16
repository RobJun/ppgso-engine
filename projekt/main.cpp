#include <ppgso/ppgso.h>
#include <memory>

#include <shaders/renderDepthMap_vert_glsl.h>
#include <shaders/renderDepthMap_frag_glsl.h>

#include <shaders/deb_vert_glsl.h>
#include <shaders/deb_frag_glsl.h>

#include <shaders/bloomf_frag_glsl.h>
#include <shaders/bloomf_vert_glsl.h>
#include <shaders/blur_vert_glsl.h>
#include <shaders/blur_frag_glsl.h>
#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>

#include "scene.h"
#include "instances/plane.h"
#include "instances/car.h"
#include "instances/cube.h"
#include "instances/Light.h"

#include "utils/renderQuad.h"

/*
* TODO:
*    simple bloom effect     - 
*    keyframes               - 
*    particle system         - 
*    movable camera          - 
*    animation               -
*    ...
*/


#define SIZE  512
#define DEBUG_SHADOW_MAPS 0


std::unique_ptr<Scene> createScene2();

class OurWindow : public ppgso::Window {
private:
    std::unique_ptr<Scene> m_scene;
    ppgso::Shader shader = { renderdepthmap_vert_glsl , renderdepthmap_frag_glsl };

    ppgso::Shader shaderBloomFinal = { bloomf_vert_glsl,bloomf_frag_glsl };
    ppgso::Shader shaderBlur = { blur_vert_glsl,blur_frag_glsl };
#if DEBUG_SHADOW_MAPS
    ppgso::Shader shader_debug = { deb_vert_glsl , deb_frag_glsl };
#endif
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];
    unsigned int hdrFBO;
    unsigned int rboDepth;
    unsigned int colorBuffers[2];
    unsigned int activeScene = 1;
public:
    OurWindow() : Window{ "projekt", SIZE, SIZE } { initWindow(); };
    OurWindow(const int& size, const double& const aspectRatio) : Window{ "projekt", size, (int)(size * aspectRatio) } { initWindow(); };

public: 
    void switchScene(std::unique_ptr<Scene> scene) {
        m_scene.reset();
        m_scene = move(scene);
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
#if DEBUG_SHADOW_MAPS
        shader_debug.use();
        shader_debug.setUniform("depthMap", 0);
#endif
        //initScene();

        //generae buffer to put rendered scene into 
        glGenFramebuffers(1, &hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glGenTextures(2, colorBuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width, this->height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            // attach texture to framebuffer
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
        }
        // create and attach depth buffer (renderbuffer)
        unsigned int rboDepth;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width, this->height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
        unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);
        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //generate frambuffer for blurring
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongColorbuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width, this->height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
            // also check if framebuffers are complete (no need for depth buffer)
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Framebuffer not complete!" << std::endl;
        }



    }

    void onIdle() override {
        // Track time
        static auto time = (float)glfwGetTime();

        // Compute time delta
        float dt = (float)glfwGetTime() - time;

        time = (float)glfwGetTime();
        if (time > 5 && activeScene == 1) {
            std::cout << "here" << std::endl;
            auto sc = createScene2();
            switchScene(move(sc));
            activeScene = 2;
        }
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
        //TODO: render to buffer
        glViewport(0, 0, width, height);
        glClearColor(.0f, 0.0f, 0.0f, 1.0f);
        // Clear depth and color buffers
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_scene->render(); // normal
                           //lights only

       glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //blur
        bool horizontal = true, first_iteration = true;
        unsigned int amount = 10;

        shaderBlur.use();
        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            shaderBlur.setUniform("horizontal", horizontal);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
            renderQuad();
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //renderhdr

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderBloomFinal.use();
        shaderBloomFinal.setUniformBuffer("scene", colorBuffers[0],0);
        shaderBloomFinal.setUniformBuffer("bloomBlur", pingpongColorbuffers[!horizontal], 1);
        shaderBloomFinal.setUniform("bloom", true);
        shaderBloomFinal.setUniform("exposure", 1.0f);
        renderQuad();
#endif
    }
};


std::unique_ptr<Scene> createScene1() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 0.1f;
    scene->m_globalLight.diffuse = 0.4f ;
    scene->m_globalLight.specular= 0.2f;
    scene->m_globalLight.color = {1,1,1};

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = -40.0f;
    scene->m_camera = move(camera);
    auto car = std::make_unique<Car>(scene.get());
    car->position = { 0,0,-10 };
    car->rotMomentum = { 0,0,0 };
    scene->m_objects.push_back(move(car));
    scene->m_objects.push_back(std::make_unique<Plane>(scene.get()));

    auto cube = std::make_unique<Cube>();
    cube->position = { 0,0,10 };
    cube->scale = { 5,5,5 };
    scene->m_objects.push_back(move(cube));

    
    auto lightCube = std::make_unique<Light>(scene.get(), "cube.obj");
    lightCube->position = { 0,2,0 };
    lightCube->scale = { 2,2,2 };
    scene->m_objects.push_back(move(lightCube));

    return scene;
}

std::unique_ptr<Scene> createScene2() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = -40.0f;
    scene->m_camera = move(camera);
    return scene;
}


int main() {
    // Initialize our window
    OurWindow window = {1200,16/9 };
    auto scene = createScene1();
    window.switchScene(move(scene));

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}