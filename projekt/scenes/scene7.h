
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
#include "../instances/Brana.h"
#include "../instances/Sphere.h"

#include "../objects/curveCamera.h"

std::unique_ptr<Scene> createScene7() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 1.f;
    scene->m_globalLight.diffuse = 0.7f;
    scene->m_globalLight.specular = 0.05f;
    scene->m_globalLight.color = { 1,1,1 };


    // Create a camera
    auto camera = std::make_unique<CurveCamera>(60.0f, 1.0f, 0.1f, 100.0f);
    //camera->position = { 18.057,5.f,-59.0985 };
    std::vector<glm::vec3> controlPoints = {
        {18.057,5.f,-59.0985},
        { -32.69 , 5, -52.8 },
        {0, 5, -26.8 },
        { 0., 5, 0 }
    };
    camera->controlPoints.push_back(controlPoints);
    camera->maxTime.push_back(30);
    scene->m_camera = move(camera);

    auto plane = std::make_unique<Plane>(scene.get());
    plane->scale = { 100, 100, 100 };

    auto gen = std::make_unique<Generator<Tree1, Tree3,Tree2,Bush>>(scene.get(), 25, &RectangelGenShape(60, 10), tranformTrees);
    gen->position = { 0,0,40 };

    auto trava = std::make_unique<Generator<Grass, Grass,Grass,Grass>>(scene.get(), 200, &RectangelGenShape(60, 100));
    auto mashroom_kvety = std::make_unique<Generator<Flower, Mushroom,Flower, Mushroom>>(scene.get(), 60, &RectangelGenShape(60, 100));

    auto tree_bush = std::make_unique<Generator<Bush, Bush,Bush,Bush>>(scene.get(), 10, &RectangelGenShape(60, 10));
    tree_bush->position = { 0,0,40 };

    auto gen2 = std::make_unique<Generator<Tree1, Tree3,Tree2,Bush>>(scene.get(), 60, &RectangelGenShape(15, 100), tranformTrees);
    gen2->position = { -20,0,-0 };
    auto gen3 = std::make_unique<Generator<Tree1, Tree3,Tree1,Tree3>>(scene.get(), 60, &RectangelGenShape(15, 100), tranformTrees);
    gen3->position = { 20,0,-0 };

    auto brana = std::make_unique<Brana>(scene.get());
    brana->position = {0,5,20};
    auto brana2 = std::make_unique<Brana>(scene.get());
    brana2->position = { 0,5,21 };
    brana2->scale = {0.12,.12,.12};
    auto brana3 = std::make_unique<Brana>(scene.get());
    brana3->position = { 0,5,22 };
    brana3->scale = { .14,.14,.14 };
    auto jaskyna = std::make_unique<Sphere>(scene.get());
    jaskyna->position = { 0,-1,22 };
    jaskyna->scale = {15,22,1};
    jaskyna->material.ambient = glm::vec3{ 0.0,	0.0, 0.0 };
    jaskyna->material.diffuse = glm::vec3{ 0.5,	0.5,0.0 };
    jaskyna->material.specular = glm::vec3{ 0.6,0.6,0.5 };;


    plane->children.push_back(move(jaskyna));
    plane->children.push_back(move(brana));
    plane->children.push_back(move(brana3));
    plane->children.push_back(move(brana2));
    plane->children.push_back(move(gen));
    plane->children.push_back(move(trava));
    plane->children.push_back(move(mashroom_kvety));
    plane->children.push_back(move(tree_bush));
    plane->children.push_back(move(gen2));
    plane->children.push_back(move(gen3));

    scene->m_objects.push_back(move(plane));

    return scene;
}