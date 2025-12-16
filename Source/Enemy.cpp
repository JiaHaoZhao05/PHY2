#include "Enemy.h"
#include "Globals.h"
#include "Application.h"
#include "Scenario.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

Enemy::Enemy(ModulePhysics* physics, int _x, int _y, Module* _listener, float _friction, float _rotation, std::vector<b2Vec2> _centerLine, ModuleAudio* _audio)
    : Car(physics->CreateRectangle(_x, _y, 32, 64, _friction, _rotation, EntityType::ENEMY, _listener, ENEMY), _listener, EntityType::ENEMY), centerLine(_centerLine)
{
}
Enemy::~Enemy()
{
}

// Load assets
bool Enemy::Start()
{
	LOG("Loading enemy");
	//centerLine = App->scenario->map1->GetCenterLine();
	texture = LoadTexture("Assets/Textures/enemy.png");
    speed = 8;
    angle = 0;
	return true;
}
// Unload assets
bool Enemy::CleanUp()
{
	LOG("Unloading enemy");

	return true;
}

// Update: draw background
bool Enemy::Update() {
	float dt = 1.0f / 60.0f; // or your actual timestep
	
    if (isActive) {
        ai.Update(this, centerLine, dt, speed, maxspeed, torque, maxtorque);
    }
    GroundFriction();
	Draw();
	return true;;
}


void Enemy::Move() {

}
void Enemy::Draw() {
	Vector2 position{ physBody->body->GetPosition().x * PIXELS_PER_METER, physBody->body->GetPosition().y * PIXELS_PER_METER };
	float scale = 1.0f;
	Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { position.x, position.y, (float)texture.width * scale, (float)texture.height * scale };
	Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
	float rotation = physBody->body->GetAngle() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}

void Enemy::GroundFriction() {
	b2Vec2 force = physBody->body->GetLinearVelocity();
	force *= -2 * physBody->body->GetFixtureList()->GetDensity() * physBody->body->GetFixtureList()->GetFriction();
	physBody->body->ApplyForceToCenter(force, true);
}
void Enemy::AddItem(Items* item) {
    EItems.push_back(item);
}

float PID::step(float error, float dt) {

	integral += error * dt;
	float derivative = (error - prevError) / dt;
	float u = kp * error + ki * integral + kd * derivative;
	prevError = error;
	return u;
}

void AIController::Update(Car* car, const std::vector<b2Vec2>& waypoints, float dt, float speed, float maxspeed, float torque, float maxtorque) {
    b2Vec2 pos = car->physBody->body->GetPosition();
    pos = b2Vec2{ (float)METERS_TO_PIXELS(pos.x), (float)METERS_TO_PIXELS(pos.y) };
    float angle = car->physBody->body->GetAngle();

    // Target waypoint
    b2Vec2 target = waypoints[currentWaypoint];
    target += car->initialPos;
    static bool debug = false;
    if (IsKeyPressed(KEY_F1)) {debug = !debug;}
    if (debug) {
        DrawCircle(target.x, target.y, 10, RED);
    }
    if ((target - pos).Length() < 200.0f && currentWaypoint + 1 < (int)waypoints.size()) {
        currentWaypoint++;
        target = waypoints[currentWaypoint];
        target += car->initialPos;
    }

    // Desired direction
    b2Vec2 dir = target - pos;
    dir.Normalize();
    float desiredAngle = atan2f(dir.y, dir.x);

    // Steering error
    if (desiredAngle > 180 * DEGTORAD) {
        desiredAngle = 180 * DEGTORAD - desiredAngle;
    }
    float steerError = fmodf(desiredAngle - angle + b2_pi, 2 * b2_pi) - b2_pi / 2;
    float steerCmd = steerPID.step(steerError, dt);

    // Speed control
    float vTarget = 5.0f;
    float maxVTarget = 5.0f;
    b2Vec2 vel = car->physBody->body->GetLinearVelocity();
    b2Vec2 forward = car->physBody->body->GetWorldVector(b2Vec2(0.0f, 1.0f));
    float vCurrent = b2Dot(vel, forward);
    float throttleCmd = throttlePID.step(speed - vCurrent, dt);

    // Apply forces
    if (car->physBody->body->GetLinearVelocity().Length() < maxspeed) {
        car->physBody->body->ApplyForceToCenter(-throttleCmd * forward, true);
    }
    if (car->physBody->body->GetAngularVelocity() < maxtorque) {
        if (steerCmd < 0) {
            car->physBody->body->ApplyTorque(-torque, true);
        }
        else if(steerCmd > 0){
            car->physBody->body->ApplyTorque(torque, true);
        }
    }
    else {
        if (steerCmd < 0) {
            car->physBody->body->ApplyTorque(torque, true);
        }
        else if (steerCmd > 0) {
            car->physBody->body->ApplyTorque(-torque, true);
        }
    }
}
void Enemy::Create() { 
    if (EItems.size() < 3) {
        create = true;
    }
}
int Enemy::ID() { 
    return id; 
}
EnemyTooth::EnemyTooth(ModulePhysics* physics, int _x, int _y, Module* _listener, float _friction, float _rotation, std::vector<b2Vec2> _centerLine, ModuleAudio* _audio)
    :Enemy(physics, _x, _y, _listener, _friction, _rotation, _centerLine, _audio)
{
}
EnemyTooth::~EnemyTooth()
{
}
bool EnemyTooth::Start(){
    LOG("Loading enemy");
    id = 2;
    texture = LoadTexture("Assets/Textures/enemy2.png");
    speed = 10; //10
    maxspeed = 38; //38
    angle = 0;
    torque = 1;
    maxtorque = 6;
    brake = 2;
    return true;
}
EnemyPsy::EnemyPsy(ModulePhysics* physics, int _x, int _y, Module* _listener, float _friction, float _rotation, std::vector<b2Vec2> _centerLine, ModuleAudio* _audio)
    :Enemy(physics, _x, _y, _listener, _friction, _rotation, _centerLine, _audio)
{
}
EnemyPsy::~EnemyPsy()
{
}
bool EnemyPsy::Start() {
    LOG("Loading enemy");
    id = 3;
    texture = LoadTexture("Assets/Textures/enemy3.png");
    speed = 11;
    maxspeed = 11;
    angle = 0;
    torque = 8;
    maxtorque = 20;
    brake = 2;
    return true;
}