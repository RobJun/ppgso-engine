
#include "../scene.h"
#include "../instances/plane.h"
#include "../instances/car.h"
#include "../instances/Tent.h"
#include "../instances/lantern.h"
#include "../instances/Tree1.h"
#include "../instances/Tree2.h"
#include "../instances/Tree3.h"
#include "../instances/Table.h"
#include "../instances/Bush.h"
#include "../instances/Log.h"
#include "../instances/Flower.h"
#include "../instances/Grass.h"
#include "../instances/Bird.h"

std::unique_ptr<Scene> createScene1() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 0.2f;
    scene->m_globalLight.diffuse = 0.1f;
    scene->m_globalLight.specular = 0.00f;
    scene->m_globalLight.color = { 1,1,1 };

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = 0;
    camera->rotate = {0,0.3,0};
    scene->m_camera = move(camera);

    auto plane = std::make_unique<Plane>(scene.get());
    plane->scale = { 20, 20, 20 };
    scene->m_objects.push_back(move(plane));
    
    auto car = std::make_unique<Car>(scene.get());
    car->position = {10,0,3};
    car->rotation = { 0,0,200 };
    car->rotMomentum = { 0,0,0 };
    scene->m_objects.push_back(move(car));

    auto tent = std::make_unique<Tent>(scene.get());
    tent->scale = { 0.2,0.2,0.2 };
    tent->position = { 5,0,10 };
    tent->rotation = { 0,0,3 };
    scene->m_objects.push_back(move(tent));

    auto table = std::make_unique<Table>(scene.get());
    table->scale = { .20,.20,.20 };
    table->position = {2,0,8};
    table->rotation = { 0,0,5};
    scene->m_objects.push_back(move(table));

    auto lantern = std::make_unique<Lantern>(scene.get());
    lantern->scale = { 0.007,0.007,0.007 };
    lantern->position = {1.5,1.45,8.5};
    scene->m_objects.push_back(move(lantern));

    auto bird = std::make_unique<Bird>(scene.get());
    bird->scale = { .03,.03,.03 };
    bird->position = {0.5,1.5,8.5};
    bird->rotation = {4.7,0,10 };
    scene->m_objects.push_back(move(bird));

    auto tree1 = std::make_unique<Tree1>(scene.get());
    tree1->scale = {.5,.5,.5 };
    tree1->position = {-5,0,0 };
    scene->m_objects.push_back(move(tree1));

    auto tree2 = std::make_unique<Tree2>(scene.get());
    tree2->scale = { .5,.5,.5 };
    tree2->position = {12,0,15};
    scene->m_objects.push_back(move(tree2));

    auto tree3 = std::make_unique<Tree3>(scene.get());
    tree3->scale = { .5,.5,.5 };
    tree3->position = {15,0,12 };
    scene->m_objects.push_back(move(tree3));

    auto bush = std::make_unique<Bush>(scene.get());
    bush->scale = { .5,.5,.5 };
    bush->position = { 5,0,12 };
    scene->m_objects.push_back(move(bush));

    auto log = std::make_unique<Log>(scene.get());
    log->scale = { .2,.2,.2 };
    log->position = { 5,0,15 };
    scene->m_objects.push_back(move(log));

    auto flower = std::make_unique<Flower>(scene.get());
    flower->scale = { .7,.7,.7 };
    flower->position = { 5,0,10 };
    scene->m_objects.push_back(move(flower));

    auto grass = std::make_unique<Grass>(scene.get());
    grass->scale = { .5,.5,.5 };
    grass->position = { 5,0,-10 };
    scene->m_objects.push_back(move(grass));


    return scene;
}