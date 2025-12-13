#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Car.h"

class Player :public Car {
public:
	Player(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, float friction)
		: Car(physics->CreateRectangle(_x,_y,5,8,PLAYER), _listener, EntityType::PLAYER)
		, texture(_texture)
	{
		
	}
	~Player();
	void Throttle(float force);
	void Turn(float torque, bool left);
	void Brake(float force);

public:
	float speed = 4;
	float torque = 0.4;
	float brake = 8;

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

public:
};