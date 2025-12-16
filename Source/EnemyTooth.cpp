#include "EnemyTooth.h"
#include "Globals.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"
EnemyTooth::EnemyTooth(ModulePhysics* physics, int _x, int _y, Module* _listener, float _rotation, std::vector<b2Vec2> _centerLine, ModuleAudio* _audio)
    :Enemy(physics, _x, _y, _listener, _rotation, _centerLine, _audio)
{
}
EnemyTooth::~EnemyTooth()
{
}
bool EnemyTooth::Start() {
    LOG("Loading enemy");
    id = 2;
    texture = LoadTexture("Assets/Textures/enemy2.png");
    speed = 10; //10
    maxspeed = 38; //38
    angle = 0;
    torque = 4;
    maxtorque = 12;
    brake = 2;
    return true;
}
