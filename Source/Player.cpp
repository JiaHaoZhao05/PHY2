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
	texture = LoadTexture("Assets/Textures/player.png");
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
	int x,y;
	physBody->GetPhysicPosition(x,y);
	pos = { (float)x,(float)y };
	Draw();
	return true;
}
//Player functions
void Player::Throttle(float force) {
	b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
	physBody->body->ApplyForceToCenter(-force * forward, true);
}
void Player::Turn(float torque, bool left) {
	if (left)physBody->body->ApplyTorque(-torque , true);
	else physBody->body->ApplyTorque(torque, true);
	
	
}
void Player::Brake(float force) {
	b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
	physBody->body->ApplyForceToCenter(force * forward, true);
}

void Player::Draw() {
	int x, y;
	physBody->GetPhysicPosition(x, y);
	Vector2 position{ (float)x, (float)y };
	float scale = 1.0f;
	Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
	Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
	float rotation = physBody->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}