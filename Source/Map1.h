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

	b2Vec2 initialPos;
	std::vector<b2Vec2> startingLine;
	float initialRotation;
	std::vector<int> chain3;
	std::vector<int> chain4;
	std::vector<b2Vec2> checkpoints;
	std::vector<b2Vec2> centerLine;
	std::vector<std::pair<std::vector<int>,b2Vec2>> boosterList;

private:
	std::vector<int> booster1;
	std::vector<int> booster2;
	std::vector<int> booster3;
	std::vector<int> booster4;
	std::vector<int> booster5;
	std::vector<int> booster6;
	std::vector<int> booster7;
	std::vector<int> booster8;
	std::vector<int> booster9;
	std::vector<int> booster10;
	std::vector<int> booster11;
	std::vector<int> booster12;

	
};