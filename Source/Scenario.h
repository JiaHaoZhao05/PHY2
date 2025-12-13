#include "Map1.h"
#include "Module.h"

class Scenario : public Module {
public:
	Scenario(Application* app, bool start_enabled = true);
	~Scenario();

	void LoadMap(Map1 map);
};