#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Car.h"
#include "Items.h"
#include <vector>
class Player :public Car {
public:
	Player(ModulePhysics* physics, int pos_x, int pos_y, Module* _listener, float _friction)
		: Car(physics->CreateRectangle(pos_x, pos_y, 32, 64, _friction, PLAYER), _listener, EntityType::PLAYER)
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

public:
	std::vector<Items*> PItems;
	b2Vec2 pos;
private:
	Texture2D texture;
	float maxspeed = 25;
	float speed = 5;

	float maxtorque = 4;
	float torque = 2.5;

	float brake = 5;
};