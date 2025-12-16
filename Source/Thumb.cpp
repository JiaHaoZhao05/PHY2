#include "Thumb.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

Thumb::Thumb(ModulePhysics* physics, int _x, int _y, Module* _listener, b2Vec2 eject, ModuleAudio* _audio)
	: Items(physics->CreateRectangle(_x, _y, 32, 32, 0, EntityType::ITEM, _listener, ITEM, MAP | ITEM | ENEMY | PLAYER, 0, 0, 1, 1), _listener, EntityType::ITEM, _audio)
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
		physBody->body->ApplyForceToCenter(initimpulse * shot, true);
		hasbeenshot = true;
	}
	if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
		if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
			shot = physBody->body->GetLinearVelocity();
			shot.Normalize();
			physBody->body->ApplyForceToCenter(force * shot, true);
		}
	}
	if (physBody->body->GetAngularVelocity() < maxtorque) {
		physBody->body->ApplyAngularImpulse(torque, true);
	}
}

void Thumb::OnCollisionWithPlayer() {}