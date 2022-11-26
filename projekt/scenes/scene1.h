
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
#include "../instances/campfire.h"
#include "../objects/particleSystem.h"

#include "../objects/generator.h"

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
    //camera->rotate = { 0,0.3,0 };
    scene->m_camera = move(camera);

<<<<<<< HEAD
    auto car = std::make_unique<Car>(scene.get());
    car->position = { 20,1,2 };
    car->rotation = { 0,0,4.7};
=======
   auto plane = std::make_unique<Plane>(scene.get());
    plane->scale = { 20, 20, 20 };
    scene->m_objects.push_back(move(plane));
    
    /*auto car = std::make_unique<Car>(scene.get());
    car->position = {10,0,3};
    car->rotation = { 0,0,200 };
>>>>>>> 0983cdf7195b3949b3f3c183a4f10ee64c08fee7
    car->rotMomentum = { 0,0,0 };
    car->scale = { 2,2,2 };

    auto tent = std::make_unique<Tent>(scene.get());
    tent->scale = { 0.3,0.3,0.3 };
    tent->position = { 5,0,4};
    tent->rotation = { 0,0,3 };
    
    auto lantern = std::make_unique<Lantern>(scene.get());
    lantern->scale = {.05,.05,.05 };
    lantern->position = {1,6,5};
    lantern->rotation = { 0,0,1 };

    auto bird = std::make_unique<Bird>(scene.get());
    bird->scale = { .22,.22,.22 };
    bird->position = {1,6,1};
    bird->rotation = { 4.7,0,12 };

    auto table = std::make_unique<Table>(scene.get());
    table->scale = { .3,.3,.3 };
    table->position = { -2,0,5 };
    table->rotation = { 0,0,5 };
    table->children.push_back(move(lantern));
    table->children.push_back(move(bird));

    /*auto tree1 = std::make_unique<Tree1>(scene.get());
    tree1->scale = { .5,.5,.5 };
    tree1->position = { -5,0,0 };

    auto tree2 = std::make_unique<Tree2>(scene.get());
    tree2->scale = { .5,.5,.5 };
    tree2->position = { 12,0,15 };

    auto tree3 = std::make_unique<Tree3>(scene.get());
    tree3->scale = { .5,.5,.5 };
    tree3->position = { 15,0,12 };

    auto bush = std::make_unique<Bush>(scene.get());
    bush->scale = { .5,.5,.5 };
    bush->position = { 5,0,12 };

    auto log = std::make_unique<Log>(scene.get());
    log->scale = { .2,.2,.2 };
    log->position = { 5,0,15 };

    auto flower = std::make_unique<Flower>(scene.get());
    flower->scale = { .7,.7,.7 };
    flower->position = { 5,0,10 };

    auto grass = std::make_unique<Grass>(scene.get());
    grass->scale = { .5,.5,.5 };
    grass->position = { 5,0,-10 };*/

    auto campfire = std::make_unique<Campfire>(scene.get());
    campfire->scale = { 1,1,1 };
    campfire->position = { 0,0,0 };
    //campfire->rotation = {1.4, 0, 0 };

<<<<<<< HEAD
    auto plane = std::make_unique<Plane>(scene.get());
    plane->scale = {30, 30, 30 };
    
    plane->children.push_back(move(car));
    plane->children.push_back(move(table));
    plane->children.push_back(move(tent));
    plane->children.push_back(move(campfire));
    /*plane->children.push_back(move(tree1));
    plane->children.push_back(move(tree2));
    plane->children.push_back(move(tree3));
    plane->children.push_back(move(log));
    plane->children.push_back(move(bush));
    plane->children.push_back(move(flower));
    plane->children.push_back(move(grass));*/
=======
    scene->m_objects.push_back(move(campfire));*/
>>>>>>> 0983cdf7195b3949b3f3c183a4f10ee64c08fee7

    scene->m_objects.push_back(move(plane));

    //auto gen = new Generator<Tree1>(scene.get(), 5, CircleGenShape(10));
    auto gen = std::make_unique<Generator<Tree1,Tree3>>(scene.get(), 10, &CircleGenShape(10));
    scene->m_objects.push_back(move(gen));

    return scene;
}