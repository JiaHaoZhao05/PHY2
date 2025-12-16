#pragma once
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Items.h"
class Thumb : public Items {
public:
	Thumb() {}
	Thumb(ModulePhysics* physics, int _x, int _y, Module* _listener, b2Vec2 eject, ModuleAudio* audio);
	~Thumb() {}
	bool Start();
	void Behave();
	bool CleanUp();
	void OnCollisionWithPlayer() override;
private:
	bool hasbeenshot = false;
	b2Vec2 shot;
	float maxtorque = 8;
	float torque = 1;
	float initimpulse = -80;
	float maxspeed = 10;
	float force = 4;
};