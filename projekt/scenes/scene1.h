
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
    scene->m_globalLight.ambient = 2.f;
    scene->m_globalLight.diffuse = 0.1f;
    scene->m_globalLight.specular = 0.00f;
    scene->m_globalLight.color = { 1,1,1 };

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = 0;
    //camera->rotate = { 0,0.3,0 };
    scene->m_camera = move(camera);

    auto car = std::make_unique<Car>(scene.get());
    car->position = { 20,1.45,2 };
    car->rotation = { 0,0,4.7 };
    car->rotMomentum = { 0,0,0 };
    car->scale = { 2,2,2 };

    auto tent = std::make_unique<Tent>(scene.get());
    tent->scale = { 0.3,0.3,0.3 };
    tent->position = { 5,0,4 };
    tent->rotation = { 0,0,3 };

    auto lantern = std::make_unique<Lantern>(scene.get());
    lantern->scale = { .025,.025,.025 };
    lantern->position = { 1,7.4,5 };
    lantern->rotation = { 0,0,1 };

    auto bird = std::make_unique<Bird>(scene.get());
    bird->scale = { .11,.11,.11 };
    bird->position = { 1,7.2,1 };
    bird->rotation = { 4.7,0,12 };

    auto table = std::make_unique<Table>(scene.get());
    table->scale = { .3,.3,.3 };
    table->position = { -2,0,5 };
    table->rotation = { 0,0,5 };
    table->children.push_back(move(lantern));
    table->children.push_back(move(bird));

    auto tree1 = std::make_unique<Tree1>(scene.get());

    auto tree3 = std::make_unique<Tree3>(scene.get());

    auto tree2 = std::make_unique<Tree2>(scene.get());

    auto bush = std::make_unique<Bush>(scene.get());

    auto flower = std::make_unique<Flower>(scene.get());

    auto grass = std::make_unique<Grass>(scene.get());

    auto campfire = std::make_unique<Campfire>(scene.get());

    auto plane = std::make_unique<Plane>(scene.get());
    plane->scale = { 30, 30, 30 };

    plane->children.push_back(move(car));
    plane->children.push_back(move(table));
    plane->children.push_back(move(tent));
    plane->children.push_back(move(campfire));


    auto gen = std::make_unique<Generator<Tree1, Tree3>>(scene.get(), 20, &RectangelGenShape(60, 10), tranformTrees);
    gen->position = { 0,0,20 };
    
    auto trava_kvety = std::make_unique<Generator<Grass, Flower>>(scene.get(), 60, &CircleGenShape(20));

    auto tree_bush = std::make_unique<Generator<Tree2, Bush>>(scene.get(), 10, &RectangelGenShape(60, 10));
    tree_bush->position = { 0,0,20};

    auto gen2 = std::make_unique<Generator<Tree1, Tree3>>(scene.get(), 20, &RectangelGenShape(10,50),tranformTrees);
    gen2->position = { -20,0,-5 };


    plane->children.push_back(move(gen));
    plane->children.push_back(move(trava_kvety));
    plane->children.push_back(move(tree_bush));
    plane->children.push_back(move(gen2));

    
    scene->m_objects.push_back(move(plane));

    return scene;
}