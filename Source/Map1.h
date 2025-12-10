#pragma once

#include "ModulePhysics.h"
#include <vector>

class Map1 {
public:
	Map1();
	Map1(std::vector<b2ChainShape> _mapChain);
	~Map1();

private:
	std::vector<b2ChainShape> mapChain;
};