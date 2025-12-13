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
	//initialize the camera
	
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
void Player::Throttle(float force, bool front) {
	if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
		if (front) {
			b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
			physBody->body->ApplyForceToCenter(-force * forward, true);
		}
		else {
			b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
			physBody->body->ApplyForceToCenter(force * forward, true);
		}
	}
}
void Player::Turn(float torque, bool left, bool turn) {
	if (turn) {
		if (abs(physBody->body->GetAngularVelocity()) < maxtorque) {
			if (left)physBody->body->ApplyTorque(-torque, true);
			else physBody->body->ApplyTorque(torque, true);
		}
	}
	else {
		if (physBody->body->GetAngularVelocity() != 0) {
			physBody->body->SetAngularVelocity(0);
		}
	}
}
void Player::Brake(float force) {
	b2Vec2 brakeF;
	if (physBody->body->GetLinearVelocity().x > 0) {
		brakeF.x = -force;
	}
	else if (physBody->body->GetLinearVelocity().x < 0) {
		brakeF.x = force;
	}
	else {
		brakeF.x = 0;
	}
	if (physBody->body->GetLinearVelocity().y > 0) {
		brakeF.y = -force;
	}
	else if (physBody->body->GetLinearVelocity().y < 0) {
		brakeF.y = force;
	}
	else {
		brakeF.y = 0;
	}
	physBody->body->ApplyForceToCenter(brakeF, true);
}

void Player::Draw() {
	Vector2 position{ GetScreenWidth() / 2,  GetScreenHeight() / 2 };
	float scale = 1.0f;
	Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
	Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
	float rotation = physBody->body->GetAngle() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	//DrawTexture(texture, GetScreenWidth() / 2 - texture.width / 2, GetScreenHeight() / 2 - texture.height / 2, WHITE);
}