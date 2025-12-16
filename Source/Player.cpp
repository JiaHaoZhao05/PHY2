#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"
#include "Player.h"

Player::Player(ModulePhysics* physics, int pos_x, int pos_y, Module* _listener, float _rotation, std::vector<b2Vec2> _checkpoints, Application* _app)
	: Car(physics->CreateRectangle(pos_x, pos_y, 32, 64, _rotation, EntityType::PLAYER, _listener, PLAYER), _listener, EntityType::PLAYER), checkpoints(_checkpoints), app(_app)
{}

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

	/*texture = LoadTexture("Assets/Textures/player.png");*/

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
	if (throttleFXCooldown.ReadSec() > 1.0f) {
		app->audio->PlayFx(engineFX-1);
		throttleFXCooldown.Start();
	}
	if (physBody->body->GetLinearVelocity().Length() < maxspeed) {
		if (front) {
			b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
			app->physics->ApplySpeed(-speed * forward, physBody);
		}
		else {
			b2Vec2 forward = physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
			app->physics->ApplySpeed(speed * forward, physBody);
		}
	}
}
void Player::Turn(bool left, bool turn) {
	if (turnFXTimePlaying.ReadSec() > 0.2f) {
		app->audio->PlayFx(turnFX-1);
		turnFXTimePlaying.Start();
	}
	if (turn) {
		if (abs(physBody->body->GetAngularVelocity()) < maxtorque) {
			if (left)app->physics->ApplyAngularSpeed(-torque, physBody);
			
			else app->physics->ApplyAngularSpeed(torque, physBody);
		}
	}
	else {
		if (physBody->body->GetAngularVelocity() != 0) {
			physBody->body->SetAngularVelocity(0);
		}
	}
}
void Player::Brake() {
	if (brakeFXCooldown.ReadSec() > 1.0f) {
		app->audio->PlayFx(brakeFX-1);
		brakeFXCooldown.Start();
	}
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
	app->physics->ApplySpeed(brakeF, physBody);
}
void Player::GroundFriction() {
	b2Vec2 force = physBody->body->GetLinearVelocity();
	force *= -1 * physBody->body->GetFixtureList()->GetDensity() * physBody->body->GetFixtureList()->GetFriction();
	app->physics->ApplySpeed(force, physBody);
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
	app->audio->PlayFx(armThrowFX-1);
	PItems.push_back(item);
}
void Player::Thumbus(Items* item) {
	app->audio->PlayFx(armThrowFX - 1);
	Thumbs.push_back(item);
}
void Player::CheckCheckpoints() {
	static bool debug = false;
	if (IsKeyPressed(KEY_F1))
	{
		debug = !debug;
	}
	if (debug) {
		DrawCircle(nextCheckpoint.x, nextCheckpoint.y, 10, BLUE);
	}
	if (currentCheckpoint + 1 < (int)checkpoints.size()){
		if ((nextCheckpoint - pos).Length() < 200.0f) {
			currentCheckpoint++;
			nextCheckpoint = checkpoints[currentCheckpoint];
			nextCheckpoint += initialPos;
		}
	}
	else if ((nextCheckpoint.x - pos.x) < 10.0f) { //END RACE
		finished = true;
		isActive = false;
	}
}


void Player::OnCollisionEnemy() {
	app->audio->PlayFx(carCollisionWithCarFX-1);
	//physBody->body->ApplyLinearImpulseToCenter({ -5,0 }, 1);
}

void Player::OnCollisionBooster(b2Vec2 dir) {
	app->audio->PlayFx(boostFX-1);
	app->physics->ApplyImpulse(b2Vec2{ dir.x * 4,dir.y * 4 }, physBody);
}

void Player::OnCollisionRoughSurface() {
	physBody->body->GetFixtureList()->SetFriction(frictionRough);
}

void Player::OnCollisionSlidingSurface() {
	physBody->body->GetFixtureList()->SetFriction(frictionSlide);
}

void Player::EndCollisionSurface() {
	physBody->body->GetFixtureList()->SetFriction(friction);
}

void Player::OnCollisionSpit(bool inside) {
	if (inside) {
		physBody->body->GetFixtureList()->SetFriction(frictionSlide);
	}
	else {
		physBody->body->GetFixtureList()->SetFriction(friction);
	}
}