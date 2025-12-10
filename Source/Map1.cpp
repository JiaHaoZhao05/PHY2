#include "Map1.h"
#include "ModulePhysics.h"
#include "Application.h"
#include "Globals.h"

Map1::Map1() {}
Map1::Map1(std::vector<b2ChainShape> _mapChain) {
	// Pivot 0, 0
	int points[12] = {
		733, 1360,
		838, 1189,
		1072, 1176,
		1202, 1322,
		1140, 1520,
		860, 1560
	};
	b2ChainShape chain1;

}
Map1::~Map1() {}