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

    int generateSpotLight(glm::vec3& position, glm::vec3& direction, glm::vec4& color, glm::vec3& phong, glm::vec3& lqc, glm::vec2& cuttoffs) {
        spotLights.push_back(std::make_unique < ppgso::light::Spot>());
        int last = spotLights.size() - 1;
        spotLights[last]->position = position;
        spotLights[last]->direction = direction;
        spotLights[last]->color = color;
        spotLights[last]->diffuse = phong.x;
        spotLights[last]->ambient = phong.y;
        spotLights[last]->specular = phong.z;
        spotLights[last]->linear = lqc.x;
        spotLights[last]->quadratic = lqc.y;
        spotLights[last]->constant = lqc.z;
        spotLights[last]->cutOff = cuttoffs.x;
        spotLights[last]->outerCutOff = cuttoffs.y;
        return last;
    }

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

    void useLights(ppgso::Shader* shader);

    void clearObjects() { m_objects.clear(); };
private:
};