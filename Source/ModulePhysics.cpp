#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleGame.h"
#include "Eye.h"
#include "Tooth.h"
#include "Spit.h"
//#include "PhysicEntity.h"

#include "p2Point.h"

#include <math.h>
#include <vector>
#define PIXELS_PER_METER 50.0f
#define METER_PER_PIXEL (1.0f / PIXELS_PER_METER)
#define METERS_TO_PIXELS(m) ((float)((m) * PIXELS_PER_METER))
#define PIXELS_TO_METERS(p) ((float)(p) / PIXELS_PER_METER)


#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f


ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	{
		/*b2EdgeShape shape;

		b2FixtureDef sd;
		sd.shape = &shape;
		sd.friction = 0.3f;

		b2BodyDef bd;
		b2Body* floor = world->CreateBody(&bd);
		shape.SetTwoSided(b2Vec2(PIXEL_TO_METERS(0.0f), PIXEL_TO_METERS((SCREEN_HEIGHT))), b2Vec2(PIXEL_TO_METERS(SCREEN_WIDTH), PIXEL_TO_METERS(SCREEN_HEIGHT)));
		floor->CreateFixture(&sd);
		shape.SetTwoSided(b2Vec2(PIXEL_TO_METERS(SCREEN_WIDTH), PIXEL_TO_METERS((SCREEN_HEIGHT))), b2Vec2(PIXEL_TO_METERS(SCREEN_WIDTH), PIXEL_TO_METERS(0.0f)));
		floor->CreateFixture(&sd);
		shape.SetTwoSided(b2Vec2(PIXEL_TO_METERS(0.0f), PIXEL_TO_METERS((0.0f))), b2Vec2(PIXEL_TO_METERS(SCREEN_WIDTH), PIXEL_TO_METERS(0.0f)));
		floor->CreateFixture(&sd);
		shape.SetTwoSided(b2Vec2(PIXEL_TO_METERS(0.0f), PIXEL_TO_METERS((0.0f))), b2Vec2(PIXEL_TO_METERS(0.0f), PIXEL_TO_METERS(SCREEN_HEIGHT)));
		floor->CreateFixture(&sd);*/
	}
	
	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);
	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if (c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			b2BodyUserData data1 = c->GetFixtureA()->GetBody()->GetUserData();
			b2BodyUserData data2 = c->GetFixtureA()->GetBody()->GetUserData();

			PhysBody* pb1 = (PhysBody*)data1.pointer;
			PhysBody* pb2 = (PhysBody*)data2.pointer;
			if (pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}
	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, float friction, float rotation, EntityType _type, Module* _listener, uint16 categoryBits, uint16 maskBits, int16 groupIndex, float bounceness, float density)
{
	PhysBody* pbody = new PhysBody();

	pbody->type = _type;

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = density;
	fixture.restitution = bounceness;

	b->CreateFixture(&fixture);

	pbody->body = b;
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, float rotation, EntityType _type, Module* _listener, uint16 categoryBits, uint16 maskBits, float friction, int16 groupIndex, float bounceness, float density)

{
	PhysBody* pbody = new PhysBody();

	pbody->type = _type;

	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);
	body.angle = rotation;

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = density;
	fixture.friction = friction;
	fixture.restitution = bounceness;

	// TODO 2: Add filter categoryBits and maskBits to fixture
	fixture.filter.categoryBits = categoryBits;
	fixture.filter.maskBits = maskBits;

	// TODO 5: Add groupIndex filter to fixture. Set default value to 0
	fixture.filter.groupIndex = groupIndex;

	b->CreateFixture(&fixture);

	pbody->body = b;
	pbody->width = (int)(width * 0.5f);
	pbody->height = (int)(height * 0.5f);
	pbody->listener = _listener;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, float rotation, EntityType _type, Module* _listener, uint16 categoryBits, uint16 maskBits, float friction, int16 groupIndex, float bounceness, float density)
{
	PhysBody* pbody = new PhysBody();

	pbody->type = _type;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.userData.pointer = reinterpret_cast<uintptr_t>(pbody);
	body.angle = rotation;

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = density;
	fixture.friction = friction;
	fixture.restitution = bounceness;

	// TODO 2: Add filter categoryBits and maskBits to fixture
	fixture.filter.categoryBits = categoryBits;
	fixture.filter.maskBits = maskBits;

	// TODO 5: Add groupIndex filter to fixture. Set default value to 0
	fixture.filter.groupIndex = groupIndex;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	pbody->body = b;
	pbody->width = (int)(width * 0.5f);
	pbody->height = (int)(height * 0.5f);
	pbody->listener = _listener;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, std::vector<int> points, uint16 categoryBits, uint16 maskBits, int16 groupIndex)
{
    PhysBody* pbody = new PhysBody();

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(
        PIXEL_TO_METERS(x),
        PIXEL_TO_METERS(y)
    );
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

    b2Body* body = world->CreateBody(&bodyDef);

    int count = points.size() / 2;
    b2Vec2* vertices = new b2Vec2[count];

    for (int i = 0; i < count; ++i)
    {
        vertices[i].x = points[i * 2]     / PIXELS_PER_METER;
        vertices[i].y = points[i * 2 + 1] / PIXELS_PER_METER;
    }

    b2ChainShape shape;
    shape.CreateLoop(vertices, count);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.friction = 0.8f;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.filter.groupIndex = groupIndex;

    body->CreateFixture(&fixtureDef);

    delete[] vertices;

    pbody->body = body;
    pbody->width = 0;
    pbody->height = 0;

    return pbody;
}

PhysBody* ModulePhysics::CreateChainSensor(int x, int y, std::vector<int> points, EntityType _type, uint16 categoryBits, uint16 maskBits, int16 groupIndex)
{
	PhysBody* pbody = new PhysBody();

	pbody->type = _type;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(
		PIXEL_TO_METERS(x),
		PIXEL_TO_METERS(y)
	);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(pbody);

	b2Body* body = world->CreateBody(&bodyDef);
	
	

	int count = points.size() / 2;
	b2Vec2* vertices = new b2Vec2[count];

	for (int i = 0; i < count; ++i)
	{
		vertices[i].x = points[i * 2] / PIXELS_PER_METER;
		vertices[i].y = points[i * 2 + 1] / PIXELS_PER_METER;
	}
	b2PolygonShape b;
	b.Set(vertices, count);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &b;
	fixtureDef.friction = 0.8f;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.filter.groupIndex = groupIndex;
	fixtureDef.isSensor = true;

	body->CreateFixture(&fixtureDef);

	delete[] vertices;

	pbody->body = body;
	pbody->width = 0;
	pbody->height = 0;

	return pbody;
}
// 
update_status ModulePhysics::PostUpdate()
{
	if (IsKeyPressed(KEY_F1))
	{
		debug = !debug;
	}
	//PostUpdateDelete when world is not locked
	for (PhysBody* pbody : bodiesToDestroy)
	{
		world->DestroyBody(pbody->body);
		delete pbody;
	}
	bodiesToDestroy.clear();

	//PostUpdateCreate when world is not locked
	for (Enemy* n : App->game->enemies) {
		if (n->create) {
			n->create = false;
			switch (n->ID()) {
			case 1:
				n->AddItem(new Eye(App->physics, n->physBody->body->GetPosition().x * PIXELS_PER_METER, n->physBody->body->GetPosition().y * PIXELS_PER_METER, this, App->audio, App->game->eyeFX));
				break;
			case 2:
				n->AddItem(new Tooth(App->physics, n->physBody->body->GetPosition().x * PIXELS_PER_METER, n->physBody->body->GetPosition().y * PIXELS_PER_METER, this, App->audio, App->game->toothFX));
				break;
			case 3:
				n->AddItem(new Spit(App->physics, n->physBody->body->GetPosition().x * PIXELS_PER_METER, n->physBody->body->GetPosition().y * PIXELS_PER_METER, this, App->audio,  App->game->spitFX));
				break;
			default:
				break;
			}
		}
	}

	if (!debug)
	{
		return UPDATE_CONTINUE;
	}
	b2Body* body_clicked = nullptr;
	b2Vec2 mouse_position = b2Vec2{ PIXEL_TO_METERS(GetMousePosition().x + App->game->player->physBody->body->GetPosition().x - (GetScreenWidth()/(2*PIXELS_PER_METER))), PIXEL_TO_METERS(GetMousePosition().y + App->game->player->physBody->body->GetPosition().y - (GetScreenHeight() / (2 * PIXELS_PER_METER)))};
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			if (body_clicked == nullptr && mouse_joint == nullptr && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				if (f->TestPoint(mouse_position)) {
					body_clicked = b;
				}

			}
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();

				DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), (float)METERS_TO_PIXELS(shape->m_radius), Color{ 0, 0, 0, 128 });
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->m_count;
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->m_vertices[i]);
					if (i > 0)
						DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->m_vertices[0]);
				DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
			}
			break;
			}
			//Select obj
		}
	}
	//Assign joint
	if (body_clicked != nullptr) {
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = body_clicked;
		def.target = mouse_position;
		def.damping = 0.5f;
		def.stiffness = 2.0f;
		def.maxForce = 100.0f * body_clicked->GetMass();
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	//DESTROY joint
	if (mouse_joint != nullptr && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		mouse_joint->SetTarget(mouse_position);
		DrawLine(METERS_TO_PIXELS(mouse_joint->GetBodyB()->GetPosition().x), METERS_TO_PIXELS(mouse_joint->GetBodyB()->GetPosition().y), METERS_TO_PIXELS(mouse_position.x), METERS_TO_PIXELS(mouse_position.y), BLACK);
	}
	else if (mouse_joint != nullptr) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
		body_clicked = nullptr;
	}
	return UPDATE_CONTINUE;
}
void ModulePhysics::QueueBodyForDestroy(PhysBody* body)
{
	bodiesToDestroy.push_back(body);
}
// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPhysicPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x);
	y = METERS_TO_PIXELS(pos.y);
}

float PhysBody::GetRotation() const
{
	return body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = (float)(x2 - x1);
			float fy = (float)(y2 - y1);
			float dist = sqrtf((fx * fx) + (fy * fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return (int)(output.fraction * dist);
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	b2BodyUserData dataA = contact->GetFixtureA()->GetBody()->GetUserData();
	b2BodyUserData dataB = contact->GetFixtureB()->GetBody()->GetUserData();

	PhysBody* physA = (PhysBody*)dataA.pointer;
	PhysBody* physB = (PhysBody*)dataB.pointer;

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void ModulePhysics::EndContact(b2Contact* contact)
{
	PhysBody* bodyA = reinterpret_cast<PhysBody*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
	PhysBody* bodyB = reinterpret_cast<PhysBody*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
	if (!bodyA || !bodyB) return;
	if (bodyA->listener) {
			bodyA->listener->EndCollision(bodyA, bodyB);
		//if (bodyB->Contains(bodyA->body->GetPosition().x, bodyA->body->GetPosition().y)) {
			//	bodyA->listener->EndCollision(bodyA, bodyB);
			//	}
	}

	if (bodyB->listener) {
			bodyB->listener->EndCollision(bodyB, bodyA);
			
		//if (bodyA->Contains(bodyB->body->GetPosition().x, bodyB->body->GetPosition().y)) {
			//	bodyB->listener->EndCollision(bodyA, bodyB);
			//}
	}
}

void ModulePhysics::ApplySpeed(b2Vec2 vec, PhysBody* body) {
	body->body->ApplyForceToCenter(vec, true);
}

void ModulePhysics::ApplyAngularSpeed(float torque, PhysBody* body) {
	body->body->ApplyTorque(torque, true);
}

void ModulePhysics::ApplyImpulse(b2Vec2 vec, PhysBody* body) {
	body->body->ApplyLinearImpulseToCenter(vec, true);
}