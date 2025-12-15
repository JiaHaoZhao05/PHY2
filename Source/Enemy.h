#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Car.h"
#include "Map1.h"

struct PID {
    float kp, ki, kd;
    float integral = 0.0f;
    float prevError = 0.0f;

    float step(float error, float dt);
};

struct AIController {
    PID steerPID{ 1.0f, 0.0f, 0.2f };
    PID throttlePID{ 0.5f, 0.0f, 0.1f };
    int currentWaypoint = 0;


	void Update(Car* car, const std::vector<b2Vec2>& waypoints, float dt);
};

class Enemy :public Car {
public:
	Enemy(ModulePhysics* physics, int _x, int _y, Module* _listener, float _friction, float _rotation, std::vector<b2Vec2> _centerLine)
		: Car(physics->CreateRectangle(_x, _y, 32, 64, _friction, _rotation, EntityType::ENEMY,_listener, ENEMY), _listener, EntityType::ENEMY), centerLine(_centerLine)
	{

	}
	~Enemy();

	void Move();
    void Draw();
    void GroundFriction();

public:
	float torque = 0.4;
	float brake = 8;
    b2Vec2 initialPos;
	bool isActive = false;
protected:
	b2Vec2 pos;
	double angle = 0;
private:
	Texture2D texture;

public:

	bool Start();
	bool Update();
	bool CleanUp();

private:
	AIController ai;
    std::vector<b2Vec2> centerLine;
};