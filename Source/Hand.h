#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Items.h"
class Hand : public Items {
public:
	Hand() {}
	Hand(ModulePhysics* physics, int _x, int _y, Module* _listener, b2Vec2 eject, ModuleAudio* audio);
	~Hand() {}
	void Behave();
	bool Start();
	bool Update();
	bool CleanUp();
	void OnCollisionWithPlayer() override;
private:
	bool hasbeenshot = false;
	b2Vec2 shot;
	float maxtorque = 8;
	float torque = 1;
	float initimpulse = -1000;
	float maxspeed = 20;
	float force = 4;
};