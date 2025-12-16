#include "EnemyPsy.h"
#include "Globals.h"
#include "Application.h"
#include "Scenario.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"
EnemyPsy::EnemyPsy(ModulePhysics* physics, int _x, int _y, Module* _listener, float _rotation, std::vector<b2Vec2> _centerLine, Application* _app)
    :Enemy(physics, _x, _y, _listener, _rotation, _centerLine, _app)
{
}
EnemyPsy::~EnemyPsy()
{
}
bool EnemyPsy::Start() {
    LOG("Loading enemy");
    id = 3;
    texture = LoadTexture("Assets/Textures/enemy3.png");
    speed = 11;
    maxspeed = 11;
    angle = 0;
    torque = 8;
    maxtorque = 20;
    brake = 2;
    return true;
}
void EnemyPsy::AddItem(Items* item) {
    EItems.push_back(item);
    b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
    physBody->body->ApplyForceToCenter(-maxspeed * 2 * forward, true);
    physBody->body->ApplyAngularImpulse(maxtorque * 2, true);
}