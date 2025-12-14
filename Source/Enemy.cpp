#include "Enemy.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

Enemy::~Enemy()
{
}

// Load assets
bool Enemy::Start()
{
	LOG("Loading enemy");
	centerLine = map1.GetCenterLine();
	texture = LoadTexture("Assets/Textures/enemy.png");
	return true;
}
// Unload assets
bool Enemy::CleanUp()
{
	LOG("Unloading enemy");

	return true;
}

// Update: draw background
bool Enemy::Update() {
	float dt = 1.0f / 60.0f; // or your actual timestep
	/*for (b2Vec2 target : centerLine) {
		target += { -5924 + (SCREEN_WIDTH / 2), -942 + (SCREEN_HEIGHT / 2)};
		DrawCircle(target.x, target.y, 10, RED);
	}*/
	ai.Update(this, centerLine, dt);
	Draw();
	return true;
}


void Enemy::Move() {

}
void Enemy::Draw() {
	Vector2 position{ physBody->body->GetPosition().x * PIXELS_PER_METER, physBody->body->GetPosition().y * PIXELS_PER_METER };
	float scale = 1.0f;
	Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
	Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
	float rotation = physBody->body->GetAngle() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}