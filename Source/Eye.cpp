#include "Eye.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

// Load assets
Eye::Eye(ModulePhysics* physics, int _x, int _y, Module* _listener, ModuleAudio* _audio)
	: Items(physics->CreateCircle(_x, _y, 16, 0.2, 0, EntityType::ITEM, _listener, ITEM, MAP | PLAYER | ENEMY, 0, 1), _listener, EntityType::ITEM, _audio)
{
	Start();
}
bool Eye::Start()
{
	LOG("Loading hand");
	sound = audio->LoadFx("Assets/Sounds/eyeCollisionFX.wav");
	texture = LoadTexture("Assets/Textures/eye.png");
	return true;
}
// Unload assets
bool Eye::CleanUp()
{
	LOG("Unloading hand");

	return true;
}

// Update: draw background
bool Eye::Update() {
	Behave();
	Draw();
	return true;
}

void Eye::Behave() {
	if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
		shot.x = rand() % 21 - 10;
		shot.y = rand() % 21 - 10;
		shot.Normalize();
		physBody->body->ApplyForceToCenter(-force * shot, true);
	}
}

void Eye::OnCollisionWithPlayer() {
	audio->PlayFx(sound-1);
}