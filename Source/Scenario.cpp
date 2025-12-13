#include "Globals.h"
#include "Application.h"
#include "Scenario.h"
#include "ModuleGame.h"

Scenario::Scenario(Application* app, bool start_enabled) : Module(app, start_enabled) {
}

Scenario::~Scenario() {}

bool Scenario::Init() {
	currentMap = 1;
	map1 = new Map1();
	return true;
	
}

update_status Scenario::Update()
{
	//Draw();
	return UPDATE_CONTINUE;
}

bool Scenario::CleanUp()
{
	return true;
}

void Scenario::LoadMap() {
	switch (currentMap) {
	case 1:
		LoadMap1();
		break;
	}
}

void Scenario::LoadMap1() {
	currentMapTex =  map1->mapTex;
	mapPos = map1->startingLine;
	chain1 = App->physics->CreateChain(mapPos.x, mapPos.y, map1->chain3);
	chain2 = App->physics->CreateChain(mapPos.x, mapPos.y, map1->chain4);
}

void Scenario::Draw() {
	//DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
	//DrawTexture(currentMapTex, mapPos.x-App->game->player->pos.x, mapPos.y-App->game->player->pos.y, WHITE);
	//DrawTexture(currentMapTex, mapPos.x, mapPos.y, WHITE);
}
