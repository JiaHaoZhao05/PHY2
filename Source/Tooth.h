#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Items.h"
class Tooth : public Items {
public:
	Tooth() {}
	Tooth(ModulePhysics* physics, int _x, int _y, Module* _listener);
	~Tooth() {}
	void Behave();
	bool Start();
	bool CleanUp();
private:
	b2Vec2 shot;
	float maxspeed = 160;
	float force = 20;
};