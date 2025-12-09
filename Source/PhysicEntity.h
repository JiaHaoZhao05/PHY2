#pragma once
#include "ModulePhysics.h"
#include "ModuleGame.h"

class PhysicEntity
{
public:

	PhysicEntity();
	PhysicEntity(PhysBody* _body, Module* _listener)
		: physBody(_body)
		, listener(_listener)
	{
		physBody->listener = listener;
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

};