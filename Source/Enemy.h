#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Car.h"

class Enemy :public Car {
public:
	Enemy(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, float friction)
		: Car(physics->CreateRectangle(_x, _y, 5, 8, ENEMY), _listener, EntityType::ENEMY)
		, texture(_texture)
	{

	}
	~Enemy();

	void Move();

public:
	float speed = 4;
	float torque = 0.4;
	float brake = 8;

protected:
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