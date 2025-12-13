#include "Map1.h"
#include "Module.h"

class Scenario : public Module {
public:
	Scenario(Application* app, bool start_enabled = true);
	~Scenario();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void LoadMap();
	void LoadMap1();
	void Draw();


public:
	Texture currentMapTex;
	Map1* map1;
	int currentMap;
	b2Vec2 mapPos;
	b2Body* body;
};