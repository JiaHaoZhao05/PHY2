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
private:
	Texture2D texture;

public:
	Car() {}
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
		return;
	}
	virtual bool CleanUp() {
		return true;
	}

public:
	EntityType type;
	PhysBody* physBody;
	Module* listener;
};