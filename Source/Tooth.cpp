#include "Tooth.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

// Load assets
Tooth::Tooth(ModulePhysics* physics, int _x, int _y, Module* _listener, ModuleAudio* _audio)
	: Items(physics->CreateRectangle(_x, _y, 32, 32, 0, EntityType::ITEM, _listener, ITEM, MAP | ENEMY | PLAYER, 0, 0, 1, 10), _listener, EntityType::ITEM, _audio)
{
	Start();
}
bool Tooth::Start()
{
	LOG("Loading Tooth");
	sound = audio->LoadFx("Assets/Sounds/toothCollisionFX.wav");
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
		physBody->body->ApplyForceToCenter(force * shot, true);
	}
}

void Tooth::OnCollisionWithPlayer() {
	audio->PlayFx(sound-1);
}