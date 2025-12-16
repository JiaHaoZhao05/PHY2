#include "Eye.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

// Load assets
Eye::Eye(ModulePhysics* physics, int _x, int _y, Module* _listener, Application* _app, unsigned int _sound)
	: Items(physics->CreateCircle(_x, _y, 16, 0.2, 0, EntityType::ITEM, _listener, ITEM, MAP | PLAYER | ENEMY, 0, 1), _listener, EntityType::ITEM, _app, _sound)
{
	Start();
}
bool Eye::Start()
{

	LOG("Loading Eye");


	texture = LoadTexture("Assets/Textures/eye.png");
	return true;
}
// Unload assets
bool Eye::CleanUp()
{
	LOG("Unloading Eye");

	return true;
}

void Eye::Behave() {
	if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
		shot.x = rand() % 21 - 10;
		shot.y = rand() % 21 - 10;
		shot.Normalize();
		app->physics->ApplySpeed(-force * shot, physBody);
	}
	else {
		app->physics->SetVelocity(b2Vec2{0,0}, physBody);
	}
}

void Eye::OnCollisionWithPlayer() {
	app->audio->PlayFx(sound-1);
}