#include "Player.h"

Player::Player(b2World* world, const b2Vec2& position)
{
    // Define the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;   // or b2_staticBody / b2_kinematicBody
    bodyDef.position = position;     // initial position in world coordinates
    bodyDef.fixedRotation = true;    // optional: prevent rotation if you want

    // Create the body in the world
    body = world->CreateBody(&bodyDef);

    // Define a shape (e.g., box for player)
    b2PolygonShape boxShape;
    boxShape.SetAsBox(0.5f, 1.0f);   // half-width, half-height

    // Define fixture (physical properties)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    // Attach fixture to body
    body->CreateFixture(&fixtureDef);
}


Player::~Player() {}

void Player::OnCollisionWithMap() 
{

}