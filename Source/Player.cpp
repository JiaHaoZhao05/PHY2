#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"
#include "Player.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	return true;
}
// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	
	return UPDATE_CONTINUE;
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