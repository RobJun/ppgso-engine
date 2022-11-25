#include "scene.h"
#include "../instances/plane.h"
#include "../instances/Tree1.h"
#include "../instances/Tree2.h"
#include "../instances/Tree3.h"
#include "../instances/Boat.h"
#include "../instances/Hat.h"
#include "../instances/Men.h"

std::unique_ptr<Scene> createScene4() {
    auto scene = std::make_unique<Scene>();
    scene->clearObjects();
    scene->m_globalLight.direction = { 0,-1,1 };
    scene->m_globalLight.ambient = 0.5f;
    scene->m_globalLight.diffuse = 0.1f;
    scene->m_globalLight.specular = 0.00f;
    scene->m_globalLight.color = { 1,1,1 };

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    camera->position.z = -40.0f;
    scene->m_camera = move(camera);

    scene->m_objects.push_back(std::make_unique<Plane>(scene.get()));

    auto boat = std::make_unique<Boat>(scene.get());
    boat->scale = { 0.02,.02,.02 };
    boat->position = { 17,-2,15 };
    scene->m_objects.push_back(move(boat));

    auto tree1 = std::make_unique<Tree1>(scene.get());
    tree1->scale = { .5,.5,.5 };
    tree1->position = { 1,0,-16 };
    scene->m_objects.push_back(move(tree1));

    auto tree2 = std::make_unique<Tree2>(scene.get());
    tree2->scale = { .5,.5,.5 };
    tree2->position = { 1,0,-14 };
    scene->m_objects.push_back(move(tree2));

    auto tree3 = std::make_unique<Tree3>(scene.get());
    tree3->scale = { .5,.5,.5 };
    tree3->position = { 1,0,-12 };
    scene->m_objects.push_back(move(tree3));

    auto bush = std::make_unique<Bush>(scene.get());
    bush->scale = { .5,.5,.5 };
    bush->position = { 5,0,-2 };
    scene->m_objects.push_back(move(bush));

    auto hat = std::make_unique<Hat>(scene.get());
    hat->scale = { .01,.01,.01 };
    hat->position = { 15,0,10 };
    scene->m_objects.push_back(move(hat));

    auto men = std::make_unique<Men>(scene.get());
    men->scale = { .51,.51,.51 };
    men->position = { 15,1.5,15 };
    scene->m_objects.push_back(move(men));

    return scene;
}