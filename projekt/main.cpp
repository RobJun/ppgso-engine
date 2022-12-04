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
#include "scenes/scene1.h"
#include "scenes/scene2.h"
#include "scenes/scene6.h"
#include "scenes/scene8.h"
#include "scenes/scene9.h"
#include "scenes/scene7.h"

#include "utils/renderQuad.h"
#include "./instances/Bat.h"

/*     TODO:
*          Objekty 3D (4B)
*               - obj subory (done)
*               - uv textury (done)
*           Dynamicka scena (1B)- 2 typy objektov ktore vynikaju zanikaju (done)
*           Proceduralna scena (2B) (done)
*           Zmeny scen (2B)
*           Hierarchicka reprezentacia sceny (4B) (done)
*           proceduralna animacia (2B) (done)
*           animacia cez klucove snimky (3B) (done)
*           Kolizie (3B) (done)
*           Simulacia vektorovych sil (2B) (asi)
*           Kamera perspektiva (1B) (done)
*               - klucove snimky (2B) (done)
*               - animacne krivky (2B)
*           Kazdy zdroj svetla (3B) (done)
*           Zmena pozicie alebo farby (1B) (done)
*           Kombinovanie svetla (2B) (done)
*           Phongov model (2B) (done)
*           Tiene (3B) (done)
*           Postprocessing (4B) (done)
*           
*/

#define SIZE  1800
#define DEBUG_SHADOW_MAPS 0

class OurWindow : public ppgso::Window {
public:
    std::vector<std::unique_ptr<Scene>> preloadedScenes;
private:
    unsigned int currentScene = 0;

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


    std::map<int, int> keys;
    float dt = 0;
public:
    OurWindow() : Window{ "projekt", SIZE, SIZE } { initWindow(); };
    OurWindow(const int& size, const double& const aspectRatio) : Window{ "projekt", size, (int)(size * aspectRatio) } { initWindow(); };
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
        dt = (float)glfwGetTime() - time;

        time = (float)glfwGetTime();
        /*if (time > 5 && activeScene == 1) {
            std::cout << "here" << std::endl;
            auto sc = createScene2();
            switchScene(move(sc));
            activeScene = 2;
        }*/
        preloadedScenes[currentScene]->update(dt);

        //generate shadowMap for direct light
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 lightSpaceMatrix = preloadedScenes[currentScene]->m_globalLight.calculateShadowMap();
        shader.use();
        shader.setUniform("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, preloadedScenes[currentScene]->m_globalLight.depthMap.getWidth(), preloadedScenes[currentScene]->m_globalLight.depthMap.getHeight());
        preloadedScenes[currentScene]->m_globalLight.depthMap.bind();
        preloadedScenes[currentScene]->renderMap(&shader);
        preloadedScenes[currentScene]->m_globalLight.depthMap.unbind();

        //generate shadowMap for spot lights
        for (auto& light : preloadedScenes[currentScene]->spotLights) {
            if (light->enabled) {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glm::mat4 lightSpaceMatrix = light->calculateShadowMap();
                shader.use();
                shader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
                glViewport(0, 0, light->depthMap.getWidth(), light->depthMap.getHeight());
                light->depthMap.bind();
                preloadedScenes[currentScene]->renderMap(&shader);
                light->depthMap.unbind(); 
            }
        }
#if DEBUG_SHADOW_MAPS
        shader_debug.use();
        shader_debug.setUniform("near_plane", preloadedScenes[currentScene]->spotLights[0]->near_plane);
        shader_debug.setUniform("far_plane", preloadedScenes[currentScene]->spotLights[0]->far_plane);
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, preloadedScenes[currentScene]->m_globalLight.depthMap.getTexture());
        glBindTexture(GL_TEXTURE_2D, preloadedScenes[currentScene]->spotLights[1]->depthMap.getTexture());
        renderQuad();
#else    
        glViewport(0, 0, width, height);
        glClearColor(.0f, 0.0f, 0.0f, 1.0f);
        // Clear depth and color buffers
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        preloadedScenes[currentScene]->render(); 


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

        shaderBloomFinal.setUniform("exposure", 0.9f);

        renderQuad();
#endif
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        // Collect key state in a map
        keys[key] = action;
        if (keys[GLFW_KEY_W]) {
            preloadedScenes[currentScene]->m_camera->position -= 30.f *dt* preloadedScenes[currentScene]->m_camera->back;
        }
        else if (keys[GLFW_KEY_S]) {
            preloadedScenes[currentScene]->m_camera->position += 30.f *dt* preloadedScenes[currentScene]->m_camera->back;
        }
        if (keys[GLFW_KEY_A])
            preloadedScenes[currentScene]->m_camera->position -= 30.f* dt* glm::normalize(glm::cross(-preloadedScenes[currentScene]->m_camera->back, preloadedScenes[currentScene]->m_camera->up));
        else if (keys[GLFW_KEY_D])
            preloadedScenes[currentScene]->m_camera->position += 30.f * dt * glm::normalize(glm::cross(-preloadedScenes[currentScene]->m_camera->back, preloadedScenes[currentScene]->m_camera->up));
        if (keys[GLFW_KEY_UP]) {
            preloadedScenes[currentScene]->m_camera->pitch += 0.5 * dt;
        }
        if (keys[GLFW_KEY_DOWN]) {
            preloadedScenes[currentScene]->m_camera->pitch -= 0.5 * dt;
        }
        if (keys[GLFW_KEY_LEFT]) {
            preloadedScenes[currentScene]->m_camera->yaw -= 0.5 * dt;
        }
        if (keys[GLFW_KEY_RIGHT]) {
            preloadedScenes[currentScene]->m_camera->yaw += 0.5 * dt;
        }
        if (keys[GLFW_KEY_P]) {
            preloadedScenes[currentScene]->m_camera->roll += 0.5 * dt;
        }



        if (keys[GLFW_KEY_1]) {
            currentScene = 0;
        }
        if (keys[GLFW_KEY_2]) {
            currentScene = 1;
        }
        if (keys[GLFW_KEY_3]) {
            currentScene = 2;
        }
        if (keys[GLFW_KEY_4]) {
            currentScene = 3;
        }
        if (keys[GLFW_KEY_5]) {
            currentScene = 4;
        }

        if (keys[GLFW_KEY_F]) {
            if (preloadedScenes[currentScene]->m_camera->lightIndex != -1) {
                if (preloadedScenes[currentScene]->m_camera->name == ppgso::light::LightName::SPOT) {
                    if (preloadedScenes[currentScene]->spotLights[preloadedScenes[currentScene]->m_camera->lightIndex]->enabled == false) {
                        preloadedScenes[currentScene]->enableLight_spot(preloadedScenes[currentScene]->m_camera->lightIndex);
                        auto bat = std::make_unique<Bat>(preloadedScenes[currentScene].get());
                        bat->scale = { 5,5,5 };
                        bat->position = preloadedScenes[currentScene]->m_camera->position - 5.f * preloadedScenes[currentScene]->m_camera->back - glm::vec3{0.6,-0.6,0};
                        bat->rotation = { -1,-1,3.14 };
                        bat->translation = { 0.3,-1,-1 };
                        preloadedScenes[currentScene]->m_objects.push_back(move(bat));

                        bat = std::make_unique<Bat>(preloadedScenes[currentScene].get());
                        bat->scale = { 5,5,5 };
                        bat->position = preloadedScenes[currentScene]->m_camera->position - 5.f*preloadedScenes[currentScene]->m_camera->back + glm::vec3{ 0.6,0.6,0 };
                        bat->rotation = { -1,1,3.14 };
                        bat->translation = { -0.3,-1,-1 };
                        preloadedScenes[currentScene]->m_objects.push_back(move(bat));
                    }
                    else
                        preloadedScenes[currentScene]->disableLight_spot(preloadedScenes[currentScene]->m_camera->lightIndex);
                }
            }
        }
    }
};

int main() {
    // Initialize our window
    OurWindow window = {800,16/9 };
    window.preloadedScenes.push_back(move(createScene1()));
    window.preloadedScenes.push_back(move(createScene2()));
    window.preloadedScenes.push_back(move(createScene6()));
    window.preloadedScenes.push_back(move(createScene7()));
    window.preloadedScenes.push_back(move(createScene9()));
    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}