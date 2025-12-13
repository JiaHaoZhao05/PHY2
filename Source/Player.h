#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Car.h"

class Player :public Car {
public:
	Player(ModulePhysics* physics, int pos_x, int pos_y, Module* _listener, float _friction)
		: Car(physics->CreateRectangle(pos_x,pos_y,32,64, _friction,PLAYER), _listener, EntityType::PLAYER)
	{
		
	}
	~Player();
	void Throttle(float force);
	void Turn(float torque, bool left);
	void Brake(float force);

public:
	float speed = PIXELS_TO_METERS(25);
	float torque = PIXELS_TO_METERS(4);
	float brake = PIXELS_TO_METERS(15);

protected:
	Camera2D camera;
	b2Vec2 pos;
	double angle;
private:
	Texture2D texture;

public:

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw();

public:
	Player* player;
};