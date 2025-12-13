#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePhysics.h"
#include "Player.h"

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	player = new Player(App->physics, initialPos.x, initialPos.y, this, 0.6f);
	player->Start();
	LOG("Loading Intro assets");

	//entities.emplace_back(new Map1())

	bool ret = true;

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	ReadInputs();
	player->Update();
	return UPDATE_CONTINUE;
}



void ModuleGame::ReadInputs() {
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
		player->Turn(player->torque, true);
	}
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
		player->Turn(player->torque, false);
	}
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
		player->Throttle(player->speed);
	}
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_SPACE)) {
		player->Brake(player->brake);
	}
}


void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB) {

}