#include "Thumb.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

Thumb::Thumb(ModulePhysics* physics, int _x, int _y, Module* _listener, b2Vec2 eject, Application* _app)
	: Items(physics->CreateRectangle(_x, _y, 32, 32, 0, EntityType::ITEM, _listener, ITEM, MAP | ITEM | ENEMY | PLAYER, 0, 0, 1, 1), _listener, EntityType::ITEM, _app)

{
	Start();
	shot = eject;
}
// Load assets
bool Thumb::Start()
{
	LOG("Loading Thumb");
	texture = LoadTexture("Assets/Textures/thumb.png");
	return true;
}
// Unload assets
bool Thumb::CleanUp()
{
	LOG("Unloading Thumb");

	return true;
}

void Thumb::Behave() {
	if (!hasbeenshot) {
		app->physics->ApplySpeed(initimpulse * shot, physBody);
		hasbeenshot = true;
	}
	if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
		if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
			shot = physBody->body->GetLinearVelocity();
			shot.Normalize();
			app->physics->ApplySpeed(force * shot, physBody);
		}
	}
	if (physBody->body->GetAngularVelocity() < maxtorque) {
		app->physics->ApplyAngularSpeed(torque, physBody);
	}
}

void Thumb::OnCollisionWithPlayer() {}