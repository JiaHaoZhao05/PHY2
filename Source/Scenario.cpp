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
	chain1 = App->physics->CreateChain(0, 0, map1->chain3, MAP);
	chain2 = App->physics->CreateChain(0, 0, map1->chain4, MAP);
	centerLine = map1->GetCenterLine();
	checkpoints = map1->GetCheckpoints();
	initialRotation = map1->initialRotation;
	EntityType boosterType;
	for (auto& booster : map1->boosterList) {
		if (booster.second.x == 1) boosterType = EntityType::BOOSTER_RIGHT;
		else if (booster.second.x == -1)boosterType = EntityType::BOOSTER_LEFT;
		else if (booster.second.y == 1)boosterType = EntityType::BOOSTER_DOWN;
		else if (booster.second.y == -1)boosterType = EntityType::BOOSTER_UP;
		App->physics->CreateChainSensor(0, 0, booster.first, boosterType, MAP, (ENEMY | PLAYER));
	}
	for (auto& roughSurface : map1->roughSurfaceList) {
		App->physics->CreateChainSensor(0, 0, roughSurface, EntityType::ROUGH_SURFACE, MAP, (ENEMY | PLAYER));
	}
}

void Scenario::Draw() {
	//DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
	//DrawTexture(currentMapTex, mapPos.x-App->game->player->pos.x, mapPos.y-App->game->player->pos.y, WHITE);
	DrawTexture(currentMapTex, 0, 0, WHITE);
}
