#include "Enemy.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

Enemy::~Enemy()
{
}

// Load assets
bool Enemy::Start()
{
	LOG("Loading player");
	//centerLine = map1->GetCenterLine();
	return true;
}
// Unload assets
bool Enemy::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
bool Enemy::Update() {
	float dt = 1.0f / 60.0f; // or your actual timestep
	//ai.Update(this, centerLine, dt);
	return true;
}


void Enemy::Move() {

}