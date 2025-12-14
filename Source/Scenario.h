#include "Map1.h"
#include "Module.h"

class Scenario : public Module {
public:
	Scenario(Application* app, bool start_enabled = true);
	~Scenario();

	bool Init();
	update_status Update();
	bool CleanUp();

	void LoadMap();
	void LoadMap1();
	void Draw();


public:
	Texture currentMapTex;
	Map1* map1;
	int currentMap;
	std::vector<b2Vec2> mapPos;
	float initialRotation;
	b2Body* body;
	PhysBody* chain1;
	PhysBody* chain2;
	std::vector<b2Vec2> centerLine;
	std::vector<b2Vec2> checkpoints;
};