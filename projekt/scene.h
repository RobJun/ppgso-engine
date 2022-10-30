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

    void useLights(ppgso::Shader* shader);

    void clearObjects() { m_objects.clear(); };
private:
};