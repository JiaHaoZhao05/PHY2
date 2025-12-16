#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Car.h"
#include "Map1.h"
#include "ModuleAudio.h"
#include "Items.h"
#include <vector>

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


	void Update(Car* car, const std::vector<b2Vec2>& waypoints, float dt, float speed, float maxspeed, float torque, float maxtorque);
};

class Enemy :public Car {
public:
	Enemy(ModulePhysics* physics, int _x, int _y, Module* _listener, float _rotation, std::vector<b2Vec2> _centerLine, Application* _app);
	~Enemy();
    void Draw();
    void GroundFriction();
	virtual void AddItem(Items* item);
	void Create(); 
	int ID(); 

	void OnCollisionBooster(b2Vec2 dir);
	void OnCollisionRoughSurface();
	void OnCollisionSlidingSurface();
	void EndCollisionSurface();
	void OnCollisionSpit(bool inside);
public:
	
	bool create = false;

	float maxspeed = 30;
	float maxtorque = 60;
	float torque = 30;
	float brake = 8;
    b2Vec2 initialPos;
	bool isActive = false;
	bool finished = false;
	std::vector<Items*> EItems;

	float friction = 0.6f;
	float frictionRough = 1.0f;
	float frictionSlide = 0.1f;
protected:
	int id = 1;
	Texture2D texture;
private:

public:
	
	virtual bool Start();
	bool Update();
	bool CleanUp();

private:
	AIController ai;
    std::vector<b2Vec2> centerLine;

};
