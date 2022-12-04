#include "../scene.h"
#include "../instances/Bat.h"
#include "../instances/Bear.h"
#include "../instances/Men.h"
#include "../instances/Plane.h"
#include "../instances/Brana.h"
#include "../instances/Jaskyna1.h"
#include "../instances/Jaskyna2.h"
#include "../instances/Jaskyna3.h"
#include "../instances/Sphere.h"

#include "../objects/keyframeCamera.h"

std::unique_ptr<Scene> createScene9() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 0.3f;
    scene->m_globalLight.diffuse = 1.f;
    scene->m_globalLight.specular = 0.7f;
    scene->m_globalLight.color = { 1,1,1 };

    int l = scene->generateSpotLight(glm::vec3(0, 4, 3), glm::vec3(0, 4, 3),
        glm::vec3{ 1,1,0.3 }, glm::vec3{ 0.8f,0.6f,0.1f },
        glm::vec3{ 0.22,0.0019,1.f }, glm::vec2{ glm::radians(12.5f) ,glm::radians(17.0f) });
    
    // Create a camera*/
    auto camera = std::make_unique<KeyframeCamera>(60.0f, 1.0f, 0.1f, 1000.0f);
    camera->position = {0,3,-80};
    camera->back = { 0,0,-1 };
    camera->up = { 0,1,0 };
    camera->lightIndex = l;
    camera->name = ppgso::light::LightName::SPOT;
    camera->addInitialKeyframes();
    //camera->addInitialKeyframes();
    ppgso::KeyFrame<glm::vec3> finalPosition;
    finalPosition.transformTo = {0,4,10};
    finalPosition.time = 20;
    finalPosition.addInterState({ 0,4,0.6}, 5.3);
    finalPosition.addInterState({ 0,4,17 }, 16.3);
    finalPosition.interpolation = ppgso::BEZIER;
    camera->k_position.addFrame(finalPosition);

    ppgso::KeyFrame<float> finalRoll;
    finalRoll.transformTo = 0;
    finalRoll.time = 2;
    finalRoll.interpolation = ppgso::POLYNOMIC;
    finalRoll.addInterState(0.05, 0.5);
    finalRoll.addInterState(0, 1);
    camera->k_roll.setResetable(true);
    camera->k_roll.addFrame(finalRoll);


    scene->m_camera = move(camera);

    auto zem = std::make_unique<Plane>(scene.get());
    zem->scale = { 90, 90, 90 };
    zem->material.ambient = glm::vec3{ 0.0,0.0, 0.0 };
    zem->material.diffuse = glm::vec3{ 0.5,0.5,0.0 };
    zem->material.specular = glm::vec3{ 0.6,0.6,0.5 };;


    auto men = std::make_unique<Men>(scene.get());
    men->scale = { 0.8,0.8,0.8 };
    men->position = { -2,0,20 };
    men->rotation = { 3.14 / 2,0,3.14 / 2 };
    scene->m_objects.push_back(move(men));

    auto bear = std::make_unique<Bear>(scene.get());
    bear->scale = { .2,.2,.2 };
    bear->position = { 0,0,20 };
    scene->m_objects.push_back(move(bear));

    auto bat = std::make_unique<Bat>(scene.get());
    bat.reset();


    for (int i = -40; i < 40; i++) {
        auto brana = std::make_unique<Brana>(scene.get());
        brana->scale = { .2,.2,.2 };
        brana->position = { 0,7,i };
        zem->children.push_back(move(brana));
    }

    auto jaskyna = std::make_unique<Sphere>(scene.get());
    jaskyna->position = { 0,0,38 };
    jaskyna->scale = {30,30,1 };
    jaskyna->material.ambient = glm::vec3{ 0.0,	0.0, 0.0 };
    jaskyna->material.diffuse = glm::vec3{ 0.5,	0.5,0.0 };
    jaskyna->material.specular = glm::vec3{ 0.6,0.6,0.5 };

    auto stalagnity = std::make_unique<Generator<Jaskyna1, Jaskyna2>>(scene.get(), 20, &RectangelGenShape(5, 80));
    stalagnity->position = {8,1,0};

    auto stalagnity2 = std::make_unique<Generator<Jaskyna1, Jaskyna2>>(scene.get(), 20, &RectangelGenShape(5, 80));
    stalagnity2->position = { -8,1,0 };


    auto lesnaLavo = std::make_unique<Generator<Tree1, Tree3>>(scene.get(), 80, &RectangelGenShape(40, 80), tranformTrees);
    lesnaLavo->position = { -40 ,0,-40 };

    auto lesnaPravo = std::make_unique<Generator<Tree1, Tree3>>(scene.get(), 80, &RectangelGenShape(40, 80),tranformTrees);
    lesnaPravo->position = { 40 ,0,-40 };

    zem->children.push_back(move(jaskyna));
    zem->children.push_back(move(stalagnity));
    zem->children.push_back(move(stalagnity2));
    scene->m_objects.push_back(move(zem));
    scene->m_objects.push_back(move(lesnaLavo));
    scene->m_objects.push_back(move(lesnaPravo));

    return scene;
}