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
	centerLine = map1.GetCenterLine();

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
	/*for (b2Vec2 target : centerLine) {
		target += { -5924 + (SCREEN_WIDTH / 2), -942 + (SCREEN_HEIGHT / 2)};
		DrawCircle(target.x, target.y, 10, RED);
	}*/
	ai.Update(this, centerLine, dt);
	return true;
}


void Enemy::Move() {

}