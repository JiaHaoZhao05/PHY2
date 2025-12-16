#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "Car.h"
#include "Items.h"
#include <vector>
#include "ModuleAudio.h"

class Player :public Car {
public:
	Player(ModulePhysics* physics, int pos_x, int pos_y, Module* _listener, float _rotation, std::vector<b2Vec2> _checkpoints, Application* app);
	~Player();
	void Throttle(bool front);
	void Turn(bool left, bool turn);
	void Brake();
	void GroundFriction();
	void AddItem(Items* item);
	void Thumbus(Items* item);

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw();
	void CheckCheckpoints();
	
	//collisions
	void OnCollisionEnemy();
	void OnCollisionBooster(b2Vec2 dir);
	void OnCollisionRoughSurface();
	void OnCollisionSlidingSurface();
	void EndCollisionSurface();
	void OnCollisionSpit(bool inside);

public:
	std::vector<Items*> PItems;
	std::vector<Items*> Thumbs;
	b2Vec2 pos;
	bool isActive = false;
	bool finished = false;

	unsigned int throttleFX;
	unsigned int brakeFX;
	unsigned int turnFX;
	unsigned int crashFX;
	unsigned int engineFX;
	unsigned int boostFX;
	unsigned int armThrowFX;
	unsigned int carCollisionWithCarFX;

private:
	Texture2D texture;
	float maxspeed = 40;
	float speed = 5;
	float maxtorque = 4;
	float torque = 2.5;
	float brake = 5;

	float friction = 0.6f;
	float frictionRough = 1.0f;
	float frictionSlide = 0.1f;

	bool insideSurface = false;


protected:

private:
	std::vector<b2Vec2> checkpoints;
	b2Vec2 nextCheckpoint;
	int currentCheckpoint;
	Application* app;
	//sounds
	
	Timer throttleFXCooldown;
	Timer brakeFXCooldown;
	Timer turnFXTimePlaying;
};