#include "../scene.h"
#include "../instances/Bat.h"

//chybaju stalagnity

std::unique_ptr<Scene> createScene8() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 2.f;
    scene->m_globalLight.diffuse = 0.1f ;
    scene->m_globalLight.specular= 0.00f;
    scene->m_globalLight.color = {1,1,1};

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    scene->m_camera = move(camera);

    scene->m_objects.push_back(std::make_unique<Plane>(scene.get()));

    auto bat = std::make_unique<Bat>(scene.get());
    bat->scale = {50,50,50};
    bat->position = {0,0,0};

    scene->m_objects.push_back(move(bat));

    return scene;
}