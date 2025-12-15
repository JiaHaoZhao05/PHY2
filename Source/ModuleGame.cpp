#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Scenario.h"
#include "Player.h"
#include "Hand.h"
ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	SetTargetFPS(60);
	App->scenario->LoadMap();
	LoadEntities();

	return ret;
}

bool ModuleGame::CleanUp()
{
	App->physics->QueueBodyForDestroy(player->physBody);
	for (Enemy* entity : enemies) {
		App->physics->QueueBodyForDestroy(entity->physBody);
	}
	enemies.clear();
	for (Items* item : player->PItems) {
		App->physics->QueueBodyForDestroy(item->physBody);
	}
	player->PItems.clear();
	
	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	App->scenario->Update();
	
	ReadInputs();
	UpdateEntities();
	if (player->finished) {
		EndGame();
	}
	return UPDATE_CONTINUE;
}

void ModuleGame::ReadInputs() {
	if (gamePaused){
		if (IsKeyPressed(KEY_ENTER)) {
			if (player->finished) RestartGame();
			else StartGame();
		}	
		return;
	}
	if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && !(IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))) {
		player->Turn(true, true);
	}
	else if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && !(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))) {
		player->Turn(false, true);
	}
	else if (IsKeyReleased(KEY_D) || IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_A) || IsKeyReleased(KEY_LEFT)) {
		player->Turn(false, false);
	}
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
		player->Throttle(true);
	}
	else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
		player->Throttle(false);
	}
	if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
		player->Brake();
	}
	if (IsKeyPressed(KEY_SPACE)) {
		if (player->PItems.size() < 3) {
			player->AddItem(new Hand(App->physics, player->pos.x, player->pos.y, this, player->physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f))));
		}
	}
}
		

void ModuleGame::UpdateEntities() {
	player->Update();
	for (Enemy* entity : enemies) {
		entity->Update();
	}
	for (Items* n : player->PItems) {
		n->Update();
		if (n->pendingToDelete) {
			App->physics->QueueBodyForDestroy(n->physBody);
			delete n;
		}
	}
}

void ModuleGame::StartGame() {
	gamePaused = false;
	timer.Start();
	player->isActive = true;
	for (Enemy* entity : enemies) {
		entity->isActive = true;
	}
}

void ModuleGame::RestartGame() {
	player->finished = false;
	player->physBody->body->SetFixedRotation(false);
	CleanUp();
	LoadEntities();
}

void ModuleGame::EndGame() {
	gamePaused = true;
	for (Enemy* entity : enemies) {
		entity->isActive = false;
	}
	player->physBody->body->SetLinearVelocity({0,0});
	player->physBody->body->SetFixedRotation(true);
}

void ModuleGame::LoadEntities() {
	float rotation = App->scenario->initialRotation;
	player = new Player(App->physics, App->scenario->mapPos[0].x, App->scenario->mapPos[0].y, this, 0.6f, rotation, App->scenario->checkpoints);
	enemies.emplace_back(new Enemy(App->physics, App->scenario->mapPos[1].x, App->scenario->mapPos[1].y, this, 0.6f, rotation, App->scenario->centerLine));
	enemies.emplace_back(new Enemy(App->physics, App->scenario->mapPos[2].x, App->scenario->mapPos[2].y, this, 0.6f, rotation, App->scenario->centerLine));

	enemies.emplace_back(new Enemy(App->physics, App->scenario->mapPos[3].x, App->scenario->mapPos[3].y, this, 0.6f, rotation, App->scenario->centerLine));

	player->Start();
	for (Enemy* entity : enemies) {
		entity->Start();
	}

}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB) {
	if (bodyA->type == EntityType::PLAYER) {

		switch (bodyB->type) {
		case(EntityType::ENEMY):
			player->OnCollissionEnemy();
			break;
		case(EntityType::ITEM):
			break;
		}
	}

}

void ModuleGame::EndCollision(PhysBody* bodyA, PhysBody* bodyB) {
	if (bodyA->type == EntityType::PLAYER) {
		switch (bodyB->type) {
		case(EntityType::ENEMY):
			break;
		case(EntityType::ITEM):
			break;
		}
	}
}