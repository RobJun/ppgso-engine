#include "scene.h"
#include "../instances/plane.h"
#include "../instances/Tree1.h"
#include "../instances/Tree2.h"
#include "../instances/Tree3.h"
#include "../instances/Bush.h"
#include "../instances/Fish.h"

//!!!CHYBAJU KVAPKY

std::unique_ptr<Scene> createScene3() {
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

    auto tree1 = std::make_unique<Tree1>(scene.get());
    tree1->scale = { .5,.5,.5 };
    tree1->position = { 1,0,-16 };
    scene->m_objects.push_back(move(tree1));

    auto tree2 = std::make_unique<Tree2>(scene.get());
    tree2->scale = { .5,.5,.5 };
    tree2->position = { 1,0,-14 };
    scene->m_objects.push_back(move(tree2));

    auto tree3 = std::make_unique<Tree3>(scene.get());
    tree3->scale = { .5,.5,.5 };
    tree3->position = { 1,0,-12 };
    scene->m_objects.push_back(move(tree3));

    auto fish = std::make_unique<Fish>(scene.get());
    fish->scale = { .2,.2,.2 };
    fish->position = { -4,0,4 };
    scene->m_objects.push_back(move(fish));

    return scene;
}