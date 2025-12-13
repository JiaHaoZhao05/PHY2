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

    float step(float error, float dt) {
        integral += error * dt;
        float derivative = (error - prevError) / dt;
        float u = kp * error + ki * integral + kd * derivative;
        prevError = error;
        return u;
    }
};

struct AIController {
    PID steerPID{ 1.0f, 0.0f, 0.2f };
    PID throttlePID{ 0.5f, 0.0f, 0.1f };
    int currentWaypoint = 0;

    void Update(Car* car, const std::vector<b2Vec2>& waypoints, float dt) {
        b2Vec2 pos = car->physBody->body->GetPosition();
        pos = b2Vec2{ (float)METERS_TO_PIXELS(pos.x), (float)METERS_TO_PIXELS(pos.y)};
        float angle = car->physBody->body->GetAngle();

        // Target waypoint
        b2Vec2 target = waypoints[currentWaypoint];
        target -= {5524+SCREEN_WIDTH/2, 600+SCREEN_HEIGHT/2}; //hardcoded
        if ((target - pos).Length() < 2.0f && currentWaypoint + 1 < (int)waypoints.size()) {
            currentWaypoint++;
            target = waypoints[currentWaypoint];
        }

        // Desired direction
        b2Vec2 dir = target - pos;
        dir.Normalize();
        float desiredAngle = atan2f(dir.y, dir.x);

        // Steering error
        float steerError = fmodf(desiredAngle - angle + b2_pi, 2 * b2_pi) - b2_pi/2;
        float steerCmd = steerPID.step(steerError, dt);

        // Speed control
        float vTarget = 5.0f;
        b2Vec2 vel = car->physBody->body->GetLinearVelocity();
        b2Vec2 forward = car->physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
        float vCurrent = b2Dot(vel, forward);
        float throttleCmd = throttlePID.step(vTarget - vCurrent, dt);

        // Apply forces
        car->physBody->body->ApplyForceToCenter(-throttleCmd * forward, true);
        car->physBody->body->ApplyTorque(steerCmd, true);

    }
};

class Enemy :public Car {
public:
	Enemy(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, float friction)
		: Car(physics->CreateRectangle(_x, _y, 32, 64, friction, ENEMY), _listener, EntityType::ENEMY)
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
	double angle = 0;
private:
	Texture2D texture;

public:

	bool Start();
	bool Update();
	bool CleanUp();

private:
	AIController ai;
    Map1 map1;
    std::vector<b2Vec2> centerLine;
};