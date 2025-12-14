#include "Hand.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

// Load assets
bool Hand::Start()
{
	LOG("Loading hand");
	texture = LoadTexture("Assets/Textures/hand.png");
	return true;
}
// Unload assets
bool Hand::CleanUp()
{
	LOG("Unloading hand");

	return true;
}

// Update: draw background
bool Hand::Update() {
	Behave();
	Draw();
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