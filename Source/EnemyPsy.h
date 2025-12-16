#pragma once
#include "Module.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"
#include "Enemy.h"
#include <vector>
class EnemyPsy :public Enemy {
public:
	EnemyPsy(ModulePhysics* physics, int _x, int _y, Module* _listener, float _rotation, std::vector<b2Vec2> _centerLine, Application* _app);
	~EnemyPsy();
	bool Start() override;
	void AddItem(Items* item) override;
public:

};