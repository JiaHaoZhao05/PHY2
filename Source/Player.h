#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
class Player :public PhysicEntity {
public:
	Player(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, float friction)
		: PhysicEntity(physics->CreateRectangle(_x,_y,5,8,PLAYER), _listener)
		, texture(_texture)
	{
		type = 0;
	}
protected:
	Camera2D camera;
	b2Vec2 pos;
	float speed;
	double angle;
private:
	Texture2D texture;
};
class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Turn();
	void Throttle();
	void Brake();
public:
	Player* player;
};