#include "scene.h"
#include "../instances/Tree1.h"
#include "../instances/Tree2.h"
#include "../instances/Tree3.h"
#include "../instances/Seagull.h"
#include "../instances/Boat.h"
#include "../instances/Bush.h"
#include "../instances/Log.h"
#include "../instances/Flower.h"
#include "../instances/Grass.h"
#include "../instances/Hat.h"
#include "../instances/Men.h"
#include "../instances/lake.h"
#include "../instances/water.h"
#include "../instances/Fish.h"


std::unique_ptr<Scene> createScene2() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 2.f;
    scene->m_globalLight.diffuse = 0.1f ;
    scene->m_globalLight.specular= 0.00f;
    scene->m_globalLight.color = {1,1,1};

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 1000.0f);
    camera->position.z = -20.0f;
    scene->m_camera = move(camera);
    
    auto hat = std::make_unique<Hat>(scene.get());
    hat->scale = {.8,.8,.8};
    hat->position = {0,240,0};
    hat->rotation = { 30,0,10 };

    auto men = std::make_unique<Men>(scene.get());
    men->scale = {20,20,20 };
    men->position = {0,133,0};
    men->rotation = { 0,0,10 };

    auto boat = std::make_unique<Boat>(scene.get());
    boat->scale = {0.015,.015,.015 };
    boat->position = {-30,-.5,15 };
    boat->children.push_back(move(men));
    boat->children.push_back(move(hat));

    auto water = std::make_unique<Water>(scene.get());
    water->scale = { 100,100,100 };
    water->material.transparency = 0.7f;

    auto fish = std::make_unique<Fish>(scene.get());
    fish->scale = {.3,.3,.3 };
    fish->position = { 0,-1,0 };

    auto obloha = std::make_unique<Generator<Seagull, Seagull>>(scene.get(), 9, &SphereGenShape(5), tranformTrees);
    obloha->position = {-20,20,25};
    obloha->rotation = { 0,0,2 };

    auto tree1 = std::make_unique<Tree1>(scene.get());
    auto tree3 = std::make_unique<Tree3>(scene.get());
    auto gen = std::make_unique<Generator<Tree1, Tree3>>(scene.get(), 200, &RectangelGenShape(200, 40), tranformTrees);
    gen->position = {0,0,100 };
    auto gen2 = std::make_unique<Generator<Tree1, Tree3>>(scene.get(), 100, &RectangelGenShape(30, 200), tranformTrees);
    gen2->position = {-100,0,0 };
    auto gen3 = std::make_unique<Generator<Tree1, Tree3>>(scene.get(), 60, &RectangelGenShape(10, 200), tranformTrees);
    gen3->position = { 130,0,0 };
    

    auto lake = std::make_unique<Lake>(scene.get());

    water->children.push_back(move(boat));
    water->children.push_back(move(fish));

    lake->children.push_back(move(gen));
    lake->children.push_back(move(gen2));
    lake->children.push_back(move(gen3));
    lake->children.push_back(move(water));

    scene->m_objects.push_back(move(obloha));
    scene->m_objects.push_back(move(lake));

    return scene;
}