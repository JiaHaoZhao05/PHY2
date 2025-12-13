#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"
#include "Player.h"



Player::~Player()
{}

// Load assets
bool Player::Start()
{
	LOG("Loading player");
	return true;
}
// Unload assets
bool Player::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
bool Player::Update()
{
	
	return true;
}
//Player functions
void Player::Throttle(float force) {
	b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
	physBody->body->ApplyForceToCenter(force * forward, true);
}
void Player::Turn(float torque, bool left) {
	physBody->body->ApplyAngularImpulse(torque * left, true);
	
}
void Player::Brake(float force) {
	b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
	physBody->body->ApplyForceToCenter(-force * forward, true);
}