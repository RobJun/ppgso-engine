
#include "../scene.h"
#include "../instances/plane.h"
#include "../instances/Tree1.h"
#include "../instances/Tree2.h"
#include "../instances/Tree3.h"
#include "../instances/Bush.h"
#include "../instances/Log.h"
#include "../instances/Flower.h"
#include "../instances/Grass.h"
#include "../instances/Mushroom.h"

//!!!CHYBA JASKYNA

std::unique_ptr<Scene> createScene7() {
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

    auto car = std::make_unique<Car>(scene.get());
    car->position = { 0,0,-10 };
    car->rotMomentum = { 0,0,0 };
    scene->m_objects.push_back(move(car));

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

    auto bush = std::make_unique<Bush>(scene.get());
    bush->scale = { .5,.5,.5 };
    bush->position = { 5,0,-2 };
    scene->m_objects.push_back(move(bush));

    auto log = std::make_unique<Log>(scene.get());
    log->scale = { .5,.5,.5 };
    log->position = { 5,0,-5 };
    scene->m_objects.push_back(move(log));

    auto flower = std::make_unique<Flower>(scene.get());
    flower->scale = { .5,.5,.5 };
    flower->position = { 5,0,-10 };
    scene->m_objects.push_back(move(flower));

    auto grass = std::make_unique<Grass>(scene.get());
    grass->scale = { .5,.5,.5 };
    grass->position = { 5,0,-10 };
    scene->m_objects.push_back(move(grass));

    auto mushroom = std::make_unique<Mushroom>(scene.get());
    mushroom->scale = { 1,1,1 };
    mushroom->position = { -8,0,-2 };
    scene->m_objects.push_back(move(mushroom));

    return scene;
}