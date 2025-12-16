#include "Tooth.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

// Load assets
Tooth::Tooth(ModulePhysics* physics, int _x, int _y, Module* _listener, Application* _app, unsigned int _sound)
	: Items(physics->CreateRectangle(_x, _y, 32, 32, 0, EntityType::ITEM, _listener, ITEM, MAP | ENEMY | PLAYER, 0, 0, 1, 10), _listener, EntityType::ITEM, _app, _sound)
{
	Start();
}
bool Tooth::Start()
{
	LOG("Loading Tooth");
	texture = LoadTexture("Assets/Textures/tooth.png");
	return true;
}
// Unload assets
bool Tooth::CleanUp()
{
	LOG("Unloading Tooth");

	return true;
}

void Tooth::Behave() {
	if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
		shot = physBody->body->GetLinearVelocity();
		shot.Normalize();
		app->physics->ApplySpeed(force * shot, physBody);
	}
}

void Tooth::OnCollisionWithPlayer() {
	app->audio->PlayFx(sound-1);
}