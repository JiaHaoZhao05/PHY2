#include "Globals.h"
#include "Application.h"
#include "Scenario.h"


Scenario::Scenario(Application* app, bool start_enabled) : Module(app, start_enabled) {}

Scenario::~Scenario() {}

bool Scenario::Init() {
	currentMap = 1;
	map1 = new Map1();
	return true;
}
// Called each loop iteration
update_status Scenario::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status Scenario::Update()
{
	Draw();
	return UPDATE_CONTINUE;
}

update_status Scenario::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
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

	currentMapTex =  LoadTexture("Assets/Maps/Map1.png");

}

void Scenario::Draw() {
	b2Vec2 v1, v2;

	//v1 = b->GetWorldPoint(shape->m_vertex0);
//	v1 = b->GetWorldPoint(shape->m_vertex1);
	//DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
	DrawTexture(currentMapTex, -5924, -942, WHITE);
}
