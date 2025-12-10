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

	PhysicEntity(PhysBody* _body, Module* _listener)
		: physBody(_body)
		, listener(_listener)
	{
		
	}

	virtual ~PhysicEntity() = default;
	virtual void Update() = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}


	PhysBody* physBody;
	Module* listener;
	int type;
	bool isSwitched = false;
	int state = 0;

};