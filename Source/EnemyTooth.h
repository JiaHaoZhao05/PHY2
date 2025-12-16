#pragma once
#include "Module.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"
#include "Enemy.h"
#include <vector>

class EnemyTooth :public Enemy {
public:
	EnemyTooth(ModulePhysics* physics, int _x, int _y, Module* _listener, float _rotation, std::vector<b2Vec2> _centerLine, ModuleAudio* _audio);
	~EnemyTooth();
	bool Start() override;
public:

};
