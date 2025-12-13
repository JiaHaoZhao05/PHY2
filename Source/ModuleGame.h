#pragma once

#include "Globals.h"
#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Map1.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void ReadInputs();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	Texture enemy1Tex; //should be in Enemy.h ?
	Player* player;
	std::vector<PhysicEntity*> enemies;

	

	Map1* map1;
	
	b2Vec2 initialPos = { SCREEN_WIDTH/2,SCREEN_HEIGHT/2 }; //player position,  needs to be in scenario
};
