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
	void CheckTimers();
	void ReadInputs();
	void UpdateEntities();
	void StartGame();
	void RestartGame();
	void EndGame();
	void LoadEntities();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void EndCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	Player* player;
	std::vector<Enemy*> enemies;
	Timer timer;
	bool gamePaused = true;
	Timer startTimer;
	bool starting = false;
	
	float bestTime;
	

	Map1* map1;
	
	b2Vec2 initialPos; //player position,  needs to be in scenario
};
