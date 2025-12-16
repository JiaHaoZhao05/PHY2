#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Items.h"
class Spit : public Items {
public:
	Spit() {}
	Spit(ModulePhysics* physics, int _x, int _y, Module* _listener, Application* app, unsigned int _sound);
	~Spit() {}
	bool Start();
	bool CleanUp();
	void Behave();
	void OnCollisionWithPlayer() override;

private:
	
};