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
	currentCheckpoint = 0;
	nextCheckpoint = checkpoints[currentCheckpoint];
	nextCheckpoint += initialPos;
	/*throttleFX = audio->LoadFx("Assets/Sounds/throttleFX.wav");
	* brakeFX = audio->LoadFx("Assets/Sounds/brakeFX.wav")
	* turnFX = audio->LoadFx("Assets/Sounds/turnFX.wav")
	* crashFX = audio->LoadFx("Assets/Sounds/crashFX.wav")
	texture = LoadTexture("Assets/Textures/player.png");*/
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
	GroundFriction();
	CheckCheckpoints();
	Draw();
	return true;
}
//Player functions
void Player::Throttle(bool front) {
	if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
		if (front) {
			b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
			physBody->body->ApplyForceToCenter(-speed * forward, true);

		}
		else {
			b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
			physBody->body->ApplyForceToCenter(speed * forward, true);
		}
	}
}
void Player::Turn(bool left, bool turn) {
	if (turn) {
		if (abs(physBody->body->GetAngularVelocity()) < maxtorque) {
			if (left)physBody->body->ApplyTorque(-torque, true);
			else physBody->body->ApplyTorque(torque, true);
			/*audio->PlayFx(turnFX - 1);*/
		}
	}
	else {
		if (physBody->body->GetAngularVelocity() != 0) {
			physBody->body->SetAngularVelocity(0);
		}
	}
}
void Player::Brake() {
	b2Vec2 brakeF;
	if (physBody->body->GetLinearVelocity().x > 0) {
		brakeF.x = -speed;
	}
	else if (physBody->body->GetLinearVelocity().x < 0) {
		brakeF.x = speed;
	}
	else {
		brakeF.x = 0;
	}
	if (physBody->body->GetLinearVelocity().y > 0) {
		brakeF.y = -speed;
	}
	else if (physBody->body->GetLinearVelocity().y < 0) {
		brakeF.y = speed;
	}
	else {
		brakeF.y = 0;
	}
	physBody->body->ApplyForceToCenter(brakeF, true);
	/*audio->PlayFx(brakeFX - 1);*/
}
void Player::GroundFriction() {
	b2Vec2 force = physBody->body->GetLinearVelocity();
	force *= -1 * physBody->body->GetFixtureList()->GetDensity() * physBody->body->GetFixtureList()->GetFriction();
	physBody->body->ApplyForceToCenter(force, true);
}
void Player::Draw() {
	Vector2 position{ physBody->body->GetPosition().x * PIXELS_PER_METER, physBody->body->GetPosition().y * PIXELS_PER_METER };
	float scale = 1.0f;
	Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
	Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
	float rotation = physBody->body->GetAngle() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}

void Player::AddItem(Items* item) {
	PItems.push_back(item);
}
void Player::CheckCheckpoints() {
	DrawCircle(nextCheckpoint.x, nextCheckpoint.y, 10, BLUE);
	if (currentCheckpoint + 1 < (int)checkpoints.size()){
		if ((nextCheckpoint - pos).Length() < 200.0f) {
			currentCheckpoint++;
			nextCheckpoint = checkpoints[currentCheckpoint];
			nextCheckpoint += initialPos;
		}
	}
	else if ((nextCheckpoint.x - pos.x) < 10.0f) { //END RACE
		DrawCircle(pos.x, pos.y, 100, GREEN);
	}
}