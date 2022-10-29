#pragma once
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <glm/glm.hpp>
#include <ppgso.h>
#include "objects/object.h"
#include "objects/camera.h"


class Scene {
private:
public:
    std::vector<std::unique_ptr<Object>> m_objects;
    std::unique_ptr<Camera> m_camera;
    ppgso::light::Directional m_globalLight;

    std::vector<std::unique_ptr<ppgso::light::Spot>> spotLights;
    std::vector <std::unique_ptr<ppgso::light::Point>> pointLights;
    int number_of_spotLights = 0;
    int number_of_pointLights = 0;
public:
    Scene() {};
    Scene(std::unique_ptr<Camera> camera) : m_camera(move(camera)) {}
    void update(float time);
    void render();
    void renderMap(ppgso::Shader* shader);


    void useGlobalLights(ppgso::Shader* shader);
    void useCamera(ppgso::Shader* shader);

    int  addLight(ppgso::light::Spot& l) {
        spotLights.push_back(std::make_unique<ppgso::light::Spot>(l));
        return spotLights.size() - 1; 
    };
    int  addLight(ppgso::light::Point& l) {
        pointLights.push_back(std::make_unique<ppgso::light::Point>(l));
        return pointLights.size() - 1;
    };

    void enableLight_spot(int index) {
        spotLights[index]->enabled = true;
        number_of_spotLights++;
    };
    void disableLight_spot(int index) { 
        spotLights[index]->enabled = false;
        number_of_spotLights--;
    };

    void enableLight_point(int index)  { 
        pointLights[index]->enabled = true;
        number_of_pointLights++;
    };
    void disableLight_point(int index) { 
        pointLights[index]->enabled = false;
        number_of_pointLights--;
    };

    void useLights(ppgso::Shader* shader) {
        shader->setUniform("number_of_point_lights",number_of_pointLights);
        shader->setUniform("number_of_spot_lights", number_of_spotLights);
        int i = 0;
        int j = 2;
        for (auto& light : spotLights) {
            if (light->enabled) {
                ppgso::light::Spot* spot = light.get();
                shader->setUniform("spotLight[" + std::to_string(i) + "].shadowMap", spot->depthMap,j);
                shader->setUniform("spotLight[" + std::to_string(i) + "].color", spot->color);
                shader->setUniform("spotLight[" + std::to_string(i) + "].color", spot->color);
                shader->setUniform("spotLight["+ std::to_string(i) + "].position", spot->position);
                shader->setUniform("spotLight[" + std::to_string(i) + "].direction", spot->direction);
                shader->setUniform("spotLight[" + std::to_string(i) + "].ambient", spot->ambient);
                shader->setUniform("spotLight[" + std::to_string(i) + "].diffuse", spot->diffuse);
                shader->setUniform("spotLight[" + std::to_string(i) + "].specular", spot->specular);
                shader->setUniform("spotLight[" + std::to_string(i) + "].constant", spot->constant);
                shader->setUniform("spotLight[" + std::to_string(i) + "].linear", spot->linear);
                shader->setUniform("spotLight[" + std::to_string(i) + "].quadratic",spot->quadratic);
                shader->setUniform("spotLight[" + std::to_string(i) + "].cutOff", glm::cos(spot->cutOff));
                shader->setUniform("spotLight[" + std::to_string(i) + "].outerCutOff", glm::cos(spot->outerCutOff));
                i++;
            }
        }

        i = 0;
        for (auto& light : pointLights) {
            if (light->enabled) {
                ppgso::light::Point* spot = light.get();
                shader->setUniform("pointL[" + std::to_string(i) + "].color", spot->position);
                shader->setUniform("pointL["+ std::to_string(i) +"].position", spot->position);
                shader->setUniform("pointL[" + std::to_string(i) + "].ambient",spot->ambient);
                shader->setUniform("pointL[" + std::to_string(i) + "].diffuse", spot->diffuse);
                shader->setUniform("pointL[" + std::to_string(i) + "].specular", spot->specular);
                shader->setUniform("pointL[" + std::to_string(i) + "].constant", spot->constant);
                shader->setUniform("pointL[" + std::to_string(i) + "].linear", spot->linear);
                shader->setUniform("pointL[" + std::to_string(i) + "].quadratic", spot->quadratic);
                i++;
            }
        }
    };

    void clearObjects() { m_objects.clear(); };
private:
};