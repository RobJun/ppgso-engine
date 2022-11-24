#include "../scene.h"
#include "../instances/Bat.h"
#include "../instances/Bear.h"
#include "../instances/Men.h"

//chybaju stalagnity

std::unique_ptr<Scene> createScene9() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 0.5f;
    scene->m_globalLight.diffuse = 0.1f;
    scene->m_globalLight.specular = 0.00f;
    scene->m_globalLight.color = { 1,1,1 };

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = -40.0f;
    scene->m_camera = move(camera);

    scene->m_objects.push_back(std::make_unique<Plane>(scene.get()));

    auto bat = std::make_unique<Bat>(scene.get());
    bat->scale = { 5,5,5 };
    bat->position = { 8,5,5 };
    scene->m_objects.push_back(move(bat));

    auto men = std::make_unique<Men>(scene.get());
    men->scale = { .51,.51,.51 };
    men->position = { 15,1.5,15 };
    scene->m_objects.push_back(move(men));

    auto bear = std::make_unique<Bear>(scene.get());
    bear->scale = { .2,.2,.2 };
    bear->position = { 0,0,0 };
    scene->m_objects.push_back(move(bear));

    return scene;
}