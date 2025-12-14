#pragma once

#include "Globals.h"
#include "timer.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>



class PhysicEntity
{
public:
	PhysicEntity() {}
	PhysicEntity(PhysBody* _body, Module* _listener, EntityType _type)
		: physBody(_body)
		, listener(_listener)
		, type(_type)
	{
		
	}

	virtual ~PhysicEntity() = default;

	virtual bool Update() {
		return true;
	}

	virtual bool Start() {
		return true;
	}

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}


	PhysBody* physBody;
	Module* listener;
	bool isSwitched = false;
	int state = 0;
	EntityType type;

};