#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePhysics.h"

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
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
	return UPDATE_CONTINUE;
}



void ModuleGame::ReadInputs() {
    if (IsKeyDown(KEY_LEFT)) { 
       
    }
	if (IsKeyDown(KEY_RIGHT)) {

	}
	if (IsKeyDown(KEY_UP)) {

	}
	if (IsKeyDown(KEY_DOWN)) {

	}
	if (IsKeyDown(KEY_SPACE)) {

	}
}


void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB) {

}