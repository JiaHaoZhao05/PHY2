#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"

class Items : public PhysicEntity {

protected:
	Texture2D texture;

public:
	Items() {}
	Items(PhysBody* _body, Module* _listener, EntityType _type);
	~Items() {}

	virtual bool Start() {
		return true;
	}
	virtual bool Update();
	virtual bool CleanUp() {
		return true;
	}
	void Draw();
	virtual void Behave() {}
	void Destroy();
};