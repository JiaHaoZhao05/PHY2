#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Items.h"
class Eye : public Items {
public:
	Eye() {}
	Eye(ModulePhysics* physics, int _x, int _y, Module* _listener, ModuleAudio* audio, unsigned int _sound);
	~Eye() {}
	bool Start();
	bool CleanUp();
	void Behave();
	void OnCollisionWithPlayer() override;

private:
	b2Vec2 shot;
	float maxspeed = 40;
	float force = 40;
};