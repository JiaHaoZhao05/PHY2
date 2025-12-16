#pragma once

#include "Globals.h"
#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyPsy.h"
#include "EnemyTooth.h"
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
	void CheckMusic();

public:

	Player* player;
	std::vector<Enemy*> enemies;
	Timer timer;
	bool gamePaused = true;
	Timer startTimer;
	bool starting = false;
	Timer musicTime;
	unsigned int music;
	float bestTime;
	bool finishedOnce = false;
	
	Texture2D countdownTex1;
	Texture2D countdownTex2;
	Texture2D countdownTex3;
	Texture2D countdownTexGO;
	Map1* map1;
	
	b2Vec2 initialPos; //player position,  needs to be in scenario
};
