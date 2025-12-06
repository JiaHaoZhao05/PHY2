#pragma once

#include "Module.h"
#include "Globals.h"
#include "box2d\box2d.h"
#include "PhysicEntity.h"
#include <vector>

using namespace std;

class Map {
public:
	Map();
	~Map();

	void BuildMap();

private:
	vector<PhysicEntity> entities;

};