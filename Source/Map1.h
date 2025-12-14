#pragma once

#include "ModulePhysics.h"
#include <vector>

class Map1 {
public:
	Map1();
	Map1(b2ChainShape _mapChain);
	~Map1();

	std::vector<b2Vec2> GetCenterLine();
	std::vector<b2Vec2> GetCheckpoints();

public:
	Texture mapTex;

	b2Vec2 startingLine;

	std::vector<int> chain3;
	std::vector<int> chain4;
	std::vector<b2Vec2> checkpoints;
private:
	std::vector<b2Vec2> leftWaypoints;
	std::vector<b2Vec2> rightWaypoints;
	std::vector<b2Vec2> centerLine;
	
};