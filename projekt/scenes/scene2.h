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
#include "../objects/Group.h"

#include "../objects/keyframeCamera.h"
#include "../objects/curveCamera.h"


std::unique_ptr<Scene> createScene2() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.size = { 150,150 };
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 1.f;
    scene->m_globalLight.diffuse = 0.7f;
    scene->m_globalLight.specular = 0.05f;
    scene->m_globalLight.color = { 1,1,1 };

    // Create a camera
    auto camera = std::make_unique<CurveCamera>(60.0f, 1.0f, 0.1f, 1000.0f);
    //camera->position = { -9.4878 ,8.4576,-123.774 };
    std::vector<glm::vec3> controlPoints = {
    { 40.3 ,8.4576,-119.809 },
    { -12,7.5,-96 },
    { 22.8,6.5,-88.8 },
    { 18.60, 5.18, -57.57 }
    };
    camera->controlPoints.push_back(controlPoints);
    camera->maxTime.push_back(10);

    controlPoints = {
        { 18.60, 5.18, -57.57 },
        { 14.4,3.86,-26.34},
        { -2.,3.,-20 },
        { -1.23, 2.607, -8.4987 }
    };
    camera->controlPoints.push_back(controlPoints);
    camera->maxTime.push_back(10);

    controlPoints = {
    {  -1.23, 2.607, -8.4987 },
    { -0.46,2.214,3.0026},
    { 123,4.2,7 },
    { 123,4.2,9 },
    { -80, 6, 10 }
    };
    camera->controlPoints.push_back(controlPoints);
    camera->maxTime.push_back(30);
    


    scene->m_camera = move(camera);
    
    auto hat = std::make_unique<Hat>(scene.get());
    hat->scale = {.8,.8,.8};
    hat->position = {0,240,0};
    hat->rotation = { 30,0,10 };

    auto men = std::make_unique<Men>(scene.get());
    men->scale = {20,20,20 };
    men->position = {0,133,0};
    men->rotation = { 0,0,10 };
    men->children.push_back(move(hat));

    auto boat = std::make_unique<Boat>(scene.get());
    boat->scale = {0.015,.015,.015 };
    boat->rotation = { 0,0,3.14 };
    boat->position = {-30,-.5,15 };
    boat->children.push_back(move(men));
    boat->translation = { 1,0,0 };

    auto fish = std::make_unique<Fish>(scene.get());
    fish->position = { 0,-.5,0 };

    auto water = std::make_unique<Water>(scene.get());
    water->scale = { 100,100,100 };
    water->material.transparency = 0.7f;


    auto obloha = std::make_unique<Group>(scene.get());
    auto vtaky = std::make_unique<Generator<Seagull, Seagull, Seagull, Seagull>>(scene.get(), 9, &SphereGenShape(5), tranformTrees);
    obloha->position = {-20,20,25};
    obloha->rotation = { 0,0,2 };
    obloha->children.push_back(move(vtaky));

    auto gen = std::make_unique<Generator<Tree1, Tree3,Bush,Tree2>>(scene.get(), 200, &RectangelGenShape(250, 40), tranformTrees);
    gen->position = {0,3,100 };
    auto gen2 = std::make_unique<Generator<Tree1, Tree3,Bush,Tree2>>(scene.get(), 100, &RectangelGenShape(30, 200), tranformTrees);
    gen2->position = {-100,3,0 };
    auto gen3 = std::make_unique<Generator<Tree1, Tree3,Bush,Tree3>>(scene.get(), 100, &RectangelGenShape(10, 200), tranformTrees);
    gen3->position = { 130,3,0 }; 

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