#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Car.h"

// PID controller
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

// Angle difference helper
float AngleDiff(float a, float b) {
    float diff = fmodf(b - a + b2_pi, 2 * b2_pi) - b2_pi;
    return diff;
}

// Simple AI controller
struct AIController {
    PID steerPID{ 1.0f, 0.0f, 0.2f };
    PID throttlePID{ 0.5f, 0.0f, 0.1f };
    int currentWaypoint = 0;

    void Update(b2Body* car, const std::vector<b2Vec2>& waypoints, float dt) {
        b2Vec2 pos = car->GetPosition();
        float angle = car->GetAngle();

        // Target waypoint
        b2Vec2 target = waypoints[currentWaypoint];
        if ((target - pos).Length() < 2.0f && currentWaypoint + 1 < (int)waypoints.size()) {
            currentWaypoint++;
            target = waypoints[currentWaypoint];
        }

        // Desired direction
        b2Vec2 dir = target - pos;
        dir.Normalize();

        float desiredAngle = atan2f(dir.y, dir.x);

        // Steering error
        float steerError = AngleDiff(angle, desiredAngle);
        float steerCmd = steerPID.step(steerError, dt);

        // Speed control
        float vTarget = 10.0f; // desired speed
        b2Vec2 vel = car->GetLinearVelocity();
        b2Vec2 forward(cosf(angle), sinf(angle));
        float vCurrent = b2Dot(vel, forward);
        float throttleCmd = throttlePID.step(vTarget - vCurrent, dt);

        // Apply forces
        car->ApplyForceToCenter(throttleCmd * forward, true);
        car->ApplyTorque(steerCmd, true);
    }
};

class Enemy :public Car {
public:
	Enemy(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, float friction)
		: Car(physics->CreateRectangle(_x, _y, 5, 8,0.6, ENEMY), _listener, EntityType::ENEMY)
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

public:
	AIController ai;

};