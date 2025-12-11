#pragma once

#include "ModulePhysics.h"
#include <vector>

class Map1 {
public:
	Map1();
	Map1(b2ChainShape _mapChain);
	~Map1();

private:
	b2ChainShape mapChain1;
};