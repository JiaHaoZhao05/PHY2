#include "Spit.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

// Load assets
Spit::Spit(ModulePhysics* physics, int _x, int _y, Module* _listener, ModuleAudio* _audio)
	: Items(physics->CreateRectangleSensor(_x, _y, 288, 288, 0, EntityType::SPIT, _listener, ITEM, MAP | ENEMY | PLAYER, 0, 0, 1, 10), _listener, EntityType::SPIT, _audio)
{
	Start();
}
bool Spit::Start()
{
	LOG("Loading Spit");
	sound = audio->LoadFx("Assets/Sounds/toothCollisionFX.wav");
	texture = LoadTexture("Assets/Textures/brainjuice.png");
	return true;
}
// Unload assets
bool Spit::CleanUp()
{
	LOG("Unloading Spit");
	return true;
}

void Spit::Behave() {}

void Spit::OnCollisionWithPlayer() {
	audio->PlayFx(sound-1);
}