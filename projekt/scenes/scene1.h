
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
#include "../instances/Mushroom.h"
#include "../instances/campfire.h"
#include "../objects/particleSystem.h"

#include "../objects/generator.h"

std::unique_ptr<Scene> createScene1() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 1.f;
    scene->m_globalLight.diffuse = 0.7f;
    scene->m_globalLight.specular = 0.05f;
    scene->m_globalLight.color = { 1,1,1 };

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position = { 28.21,4,-33.6228 };
    camera->yaw = 2.0656;
    scene->m_camera = move(camera);

    auto car = std::make_unique<Car>(scene.get());
    car->rotation = { 0,0,4.7 };
    car->rotMomentum = { 0,0,0 };
    car->scale = { 2,2,2 };

    ppgso::KeyFrame<glm::vec3> firstPosition;
    firstPosition.transformTo = {40,1.45,-20};
    firstPosition.time = 0;
    firstPosition.interpolation = ppgso::CONSTANT;
    car->translateFrames.addFrame(firstPosition);
   
    ppgso::KeyFrame<glm::vec3> finalPosition;
    finalPosition.transformTo = { 20,1.45,2 };
    finalPosition.time = 10;
    finalPosition.addInterState({ 30,1.45,-10 }, 5.3);
    finalPosition.addInterState({ 25,1.45,0 }, 8.3);
    finalPosition.interpolation = ppgso::POLYNOMIC;
    car->translateFrames.addFrame(finalPosition);

    ppgso::KeyFrame<glm::vec3> firstRotation;
    firstRotation.transformTo = { 0,0,0 };
    firstRotation.time = 7;
    firstRotation.interpolation = ppgso::CONSTANT;
    car->rotationFrames.addFrame(firstRotation);

    ppgso::KeyFrame<glm::vec3> finalRotation;
    finalRotation.transformTo = { 0,0,4.7-2*3.14 };
    finalRotation.time = 10;
    finalRotation.interpolation = ppgso::LINEAR;
    car->rotationFrames.addFrame(finalRotation);

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


    auto gen = std::make_unique<Generator<Tree1, Tree3,Tree2,Bush>>(scene.get(), 40, &RectangelGenShape(60, 10), tranformTrees);
    gen->position = { 0,0,20 };
    
    auto trava_kvety = std::make_unique<Generator<Grass, Flower, Mushroom>>(scene.get(), 90, &RectangelGenShape(60,60));


    auto gen2 = std::make_unique<Generator<Tree1, Tree3,Bush,Tree2>>(scene.get(), 20, &RectangelGenShape(10,40),tranformTrees);
    gen2->position = { -20,0,-5 };

    scene->m_objects.push_back(move(plane));
    scene->m_objects.push_back(move(gen));
    scene->m_objects.push_back(move(trava_kvety));
    scene->m_objects.push_back(move(gen2));
    return scene;
}