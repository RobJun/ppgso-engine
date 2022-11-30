#include "../scene.h"
#include "../instances/Bat.h"
#include "../instances/Bear.h"
#include "../instances/Men.h"
#include "../instances/Plane.h"
#include "../instances/Brana.h"
#include "../instances/Jaskyna1.h"
#include "../instances/Jaskyna2.h"
#include "../instances/Jaskyna3.h"
#include "../instances/Sphere.h"

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
    camera->position = { 0,0,0 };
    scene->m_camera = move(camera);

    auto zem = std::make_unique<Plane>(scene.get());
    zem->scale = { 15, 40, 15 };
    zem->material.ambient = glm::vec3{ 0.0,0.0, 0.0 };
    zem->material.diffuse = glm::vec3{ 0.5,0.5,0.0 };
    zem->material.specular = glm::vec3{ 0.6,0.6,0.5 };;

    /*auto bat = std::make_unique<Bat>(scene.get());
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
    scene->m_objects.push_back(move(bear));*/


    for (int i = -40; i < 40; i++) {
        auto brana = std::make_unique<Brana>(scene.get());
        brana->scale = { .2,.2,.2 };
        brana->position = { 0,7,i };
        zem->children.push_back(move(brana));
    }

    auto jaskyna = std::make_unique<Sphere>(scene.get());
    jaskyna->position = { 0,0,38 };
    jaskyna->scale = {30,30,1 };
    jaskyna->material.ambient = glm::vec3{ 0.0,	0.0, 0.0 };
    jaskyna->material.diffuse = glm::vec3{ 0.5,	0.5,0.0 };
    jaskyna->material.specular = glm::vec3{ 0.6,0.6,0.5 };

    auto stalagnity = std::make_unique<Generator<Jaskyna1, Jaskyna2>>(scene.get(), 20, &RectangelGenShape(2, 80));
    stalagnity->position = {8,1,0};

    auto stalagnity2 = std::make_unique<Generator<Jaskyna1, Jaskyna2>>(scene.get(), 20, &RectangelGenShape(5, 80));
    stalagnity2->position = { -8,1,0 };

    zem->children.push_back(move(jaskyna));
    zem->children.push_back(move(stalagnity));
    zem->children.push_back(move(stalagnity2));
    scene->m_objects.push_back(move(zem));

    return scene;
}