#pragma once

#include "Car.h"

class Player{
public:

	Player();
	Player(b2World* world, const b2Vec2& position);
	~Player();
	
	void OnCollisionWithMap();
	
private:
	
	b2Body* body;
	b2Vec2 position;
	b2Vec2 velocity;
	
};