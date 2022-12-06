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

#include "../objects/curveCamera.h"


std::unique_ptr<Scene> createScene6() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.size = { 40,40 };
    scene->m_globalLight.ambient = 1.f;
    scene->m_globalLight.diffuse = 0.7f;
    scene->m_globalLight.specular = 0.05f;
    scene->m_globalLight.color = { 1,1,1 };

    // Create a camera
    auto camera = std::make_unique<CurveCamera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position = { -.54,4.2,7.29 };
    std::vector<glm::vec3> controlPoints = {
    { 20 ,4.2,-30 },
    { -3.6,4.2,0 },
   { -1,4.2,3.1 },
   { -.54, 4.2, 7.29 }
    };
    camera->controlPoints.push_back(controlPoints);
    camera->maxTime.push_back(20);
    scene->m_camera = move(camera);

    auto plane = std::make_unique<Plane>(scene.get());
    plane->scale = {30, 50, 30 };

    auto chicken = std::make_unique<Chicken>(scene.get());
    chicken->scale = {0.5,0.5,0.5};
    chicken->position = { 0,4.5,0 };

    auto fox = std::make_unique<Fox>(scene.get());
    fox->scale = {0.2,0.2,0.2};
    fox->position = {4,0,20};
    fox->rotation = { 0,0,-2 };
    fox->children.push_back(move(chicken));

    auto rabbit = std::make_unique<Rabbit>(scene.get());

    auto tree_Apple = std::make_unique<Tree2>(scene.get());
    tree_Apple->scale = { .5,.5,.5 };
    tree_Apple->position = {0,0,20};

    auto apple = std::make_unique<Apple>(scene.get());
    apple->scale = { .005,.005,.005 };
    apple->position = {-2.7,3,20};
    tree_Apple->children.push_back(move(apple));

    auto gen = std::make_unique<Generator<Tree1, Tree3>>(scene.get(), 25, &RectangelGenShape(60, 10), tranformTrees);
    gen->position = {0,0,40};

    auto trava = std::make_unique<Generator<Grass, Grass>>(scene.get(), 200, &RectangelGenShape(60,100));
    auto mashroom_kvety = std::make_unique<Generator<Flower, Mushroom>>(scene.get(), 60, &RectangelGenShape(60,100));

    auto tree_bush = std::make_unique<Generator<Bush, Bush>>(scene.get(), 10, &RectangelGenShape(60, 10));
    tree_bush->position = { 0,0,40 };

    auto gen2 = std::make_unique<Generator<Tree1, Tree3>>(scene.get(), 30, &RectangelGenShape(15, 100), tranformTrees);
    gen2->position = {-20,0,-0 };
    auto gen3 = std::make_unique<Generator<Tree1, Tree3>>(scene.get(), 30, &RectangelGenShape(15, 100), tranformTrees);
    gen3->position = { 30,0,-0 };

    plane->children.push_back(move(tree_Apple));
    plane->children.push_back(move(gen));
    plane->children.push_back(move(trava));
    plane->children.push_back(move(mashroom_kvety));
    plane->children.push_back(move(tree_bush));
    plane->children.push_back(move(gen2));
    plane->children.push_back(move(gen3));
    
    scene->m_objects.push_back(move(plane));

    return scene;
}