#include "Map1.h"
#include "ModulePhysics.h"
#include "Application.h"
#include "Globals.h"

Map1::Map1() {
	b2Vec2 pts[6];
	pts[0] = { 733, 1360 };
	pts[1] = { 838, 1189 };
	pts[2] = { 1072, 1176 };
	pts[3] = { 1202, 1322 };
	pts[4] = { 1140, 1520 };
	pts[5] = { 860, 1560 };

	mapChain1.CreateLoop(pts, 6);
}
Map1::Map1(b2ChainShape _mapChain) {

	

}
Map1::~Map1() {}