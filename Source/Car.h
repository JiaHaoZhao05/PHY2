#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"

class Car : public PhysicEntity {

protected:
	b2Vec2 pos;
	float speed;
	double angle;
	float friction;
	float frictionRough;
	float frictionSlide;
	bool insideSurface;

private:
	Texture2D texture;

public:

	Car(PhysBody* _body, Module* _listener, EntityType _type)
		: physBody(_body)
		, listener(_listener)
		, type(_type)
	{

	}
	~Car(){}

	virtual bool Start() {
		return true;
	}
	virtual bool Update() {
		return true;
	}
	virtual bool CleanUp() {
		return true;
	}

public:
	b2Vec2 initialPos;
	EntityType type;
	PhysBody* physBody;
	Module* listener;
	Application* app;
};