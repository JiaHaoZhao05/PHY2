#include "Hand.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

Hand::Hand(ModulePhysics* physics, int _x, int _y, Module* _listener, b2Vec2 eject, ModuleAudio* _audio)
	: Items(physics->CreateRectangle(_x, _y, 32, 64, 0, EntityType::ITEM, _listener, ITEM, 0, MAP | ENEMY, 0, 0.8), _listener, EntityType::ITEM, _audio)

{
	Start();
	shot = eject;
}
// Load assets
bool Hand::Start()
{
	LOG("Loading Hand");
	texture = LoadTexture("Assets/Textures/hand.png");
	return true;
}
// Unload assets
bool Hand::CleanUp()
{
	LOG("Unloading Hand");

	return true;
}

void Hand::Behave() {
	if (!hasbeenshot) {
		physBody->body->ApplyForceToCenter(-initimpulse * shot, true);
		hasbeenshot = true;
	}
	if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
		b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
		physBody->body->ApplyForceToCenter(-force * forward, true);
	}
	if (physBody->body->GetAngularVelocity() < maxtorque) {
		physBody->body->ApplyAngularImpulse(torque, true);
	}
}

void Hand::OnCollisionWithPlayer() {}