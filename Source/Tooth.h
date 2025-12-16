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
	Tooth(ModulePhysics* physics, int _x, int _y, Module* _listener, ModuleAudio* _audio);
	~Tooth() {}
	void Behave();
	bool Start();
	bool CleanUp();
	void OnCollisionWithPlayer() override;
private:
	b2Vec2 shot;
	float maxspeed = 110;
	float force = 20;
};