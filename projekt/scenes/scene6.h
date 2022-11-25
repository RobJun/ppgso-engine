#include "../scene.h"
#include "../instances/plane.h"
#include "../instances/Fox.h"
#include "../instances/Chicken.h"
#include "../instances/Mushroom.h"
#include "../instances/Rabbit.h"
#include "../instances/Tree1.h"
#include "../instances/Tree2.h"
#include "../instances/Tree3.h"
#include "../instances/Bush.h"
#include "../instances/Log.h"
#include "../instances/Flower.h"
#include "../instances/Grass.h"
#include "../instances/Bird.h"
#include "../instances/Apple.h"


std::unique_ptr<Scene> createScene6() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 0.5f;
    scene->m_globalLight.diffuse = 0.1f ;
    scene->m_globalLight.specular= 0.00f;
    scene->m_globalLight.color = {1,1,1};

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = -10.f;
    camera->rotate = { 0,0.8,0 };
    scene->m_camera = move(camera);

    auto plane = std::make_unique<Plane>(scene.get());
    plane->scale = { 20, 20, 20 };
    scene->m_objects.push_back(move(plane));


    /*auto fox = std::make_unique<Fox>(scene.get());
    fox->scale = {0.3,0.3,0.3};
    fox->position = {-8,0,-8};
    scene->m_objects.push_back(move(fox));

    auto chicken = std::make_unique<Chicken>(scene.get());
    chicken->scale = {0.2,0.2,0.2};
    chicken->position = { 2,0,2 };
    scene->m_objects.push_back(move(chicken));

    auto mushroom = std::make_unique<Mushroom>(scene.get());
    mushroom->scale = {1,1,1};
    mushroom->position = { -8,0,-2 };
    scene->m_objects.push_back(move(mushroom));

    auto rabbit = std::make_unique<Rabbit>(scene.get());
    rabbit->scale = { .2,.2,.2 };
    rabbit->position = { -8,0.3,-12 };
    scene->m_objects.push_back(move(rabbit));

    auto tree1 = std::make_unique<Tree1>(scene.get());
    tree1->scale = {.5,.5,.5 };
    tree1->position = { 1,0,-16 };
    scene->m_objects.push_back(move(tree1));*/

    auto tree2 = std::make_unique<Tree2>(scene.get());
    tree2->scale = { .5,.5,.5 };
    tree2->position = { 0,0,0};
    scene->m_objects.push_back(move(tree2));

    /*auto tree3 = std::make_unique<Tree3>(scene.get());
    tree3->scale = { .5,.5,.5 };
    tree3->position = { 1,0,-12 };
    scene->m_objects.push_back(move(tree3));

    auto bush = std::make_unique<Bush>(scene.get());
    bush->scale = { .5,.5,.5 };
    bush->position = {5,0,-2 };
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
    scene->m_objects.push_back(move(grass));*/

    auto apple = std::make_unique<Apple>(scene.get());
    apple->scale = { .005,.005,.005 };
    apple->position = {-2.7,2.9,0.8};
    scene->m_objects.push_back(move(apple));

    return scene;
}