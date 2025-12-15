#pragma once

#include "Module.h"
#include "Globals.h"

#include "box2d\box2d.h"
#include <vector>
#define GRAVITY_X 0.0f
#define GRAVITY_Y 0.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
enum ObjectCategories {
	PLAYER = 1,
	ENEMY = 2,
	ITEM = 4,
	DAMPSURFACE = 8,
	BOOSTER = 16,
	MAP = 32,

};
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{
	}

	//void GetPosition(int& x, int& y) const;
	void GetPhysicPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	EntityType type;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height, float friction, float rotation, EntityType _type, Module* _listener, uint16 categoryBits, uint16 maskBits = 0xFFFF, int16 groupIndex = 0, float bounceness = 0);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, std::vector<int> points, uint16 categoryBits, uint16 maskBits = 0xFFFF, int16 groupIndex = 0);
	PhysBody* CreateChainSensor(int x, int y, std::vector<int> points, EntityType _type, uint16 categoryBits, uint16 maskBits = 0xFFFF, int16 groupIndex = 0);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);


	void QueueBodyForDestroy(PhysBody* body);

private:
	std::vector<PhysBody*> bodiesToDestroy;
	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};