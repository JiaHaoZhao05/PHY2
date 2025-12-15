#include "Enemy.h"
#include "Globals.h"
#include "Application.h"
#include "Scenario.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

Enemy::~Enemy()
{
}

// Load assets
bool Enemy::Start()
{
	LOG("Loading enemy");
	//centerLine = App->scenario->map1->GetCenterLine();
	texture = LoadTexture("Assets/Textures/enemy.png");
    speed = 5;
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
	GroundFriction();
    if (isActive) {
        ai.Update(this, centerLine, dt, speed, maxspeed, torque, maxtorque);
    }

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
	force *= -1 * physBody->body->GetFixtureList()->GetDensity() * physBody->body->GetFixtureList()->GetFriction();
	physBody->body->ApplyForceToCenter(force, true);
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
    DrawCircle(target.x, target.y, 10, RED);
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
}
EnemyTooth::~EnemyTooth()
{
}
bool EnemyTooth::Start(){
    LOG("Loading enemy");
    texture = LoadTexture("Assets/Textures/enemy2.png");
    speed = 10;
    maxspeed = 19;
    angle = 0;
    torque = 3;
    maxtorque = 6;
    brake = 2;
    return true;
}