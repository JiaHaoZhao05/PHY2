#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Car.h"
#include "Items.h"
#include <vector>
#include "ModuleAudio.h"

class Player :public Car {
public:
	Player(ModulePhysics* physics, int pos_x, int pos_y, Module* _listener, float _friction, float _rotation, std::vector<b2Vec2> _checkpoints, ModuleAudio* _audio)
		: Car(physics->CreateRectangle(pos_x,pos_y,32,64, _friction, _rotation, EntityType::PLAYER,_listener,PLAYER), _listener, EntityType::PLAYER), checkpoints(_checkpoints), audio(_audio)
	{

	}
	~Player();
	void Throttle(bool front);
	void Turn(bool left, bool turn);
	void Brake();
	void GroundFriction();
	void AddItem(Items* item);

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw();
	void CheckCheckpoints();
	

	void OnCollissionEnemy();

public:
	std::vector<Items*> PItems;
	b2Vec2 pos;
	bool isActive = false;
	bool finished = false;
private:
	Texture2D texture;
	float maxspeed = 25;
	float speed = 5;

	float maxtorque = 4;
	float torque = 2.5;

	float brake = 5;
protected:

private:
	std::vector<b2Vec2> checkpoints;
	b2Vec2 nextCheckpoint;
	int currentCheckpoint;
	ModuleAudio* audio;
	//sounds
	unsigned int throttleFX;
	unsigned int brakeFX;
	unsigned int turnFX;
	unsigned int crashFX;
};