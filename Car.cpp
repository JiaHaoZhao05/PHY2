/*#include "Car.h"

//================ FixtureUserData ====================
FixtureUserData::FixtureUserData(fixtureUserDataType type) : m_type(type) {}
fixtureUserDataType FixtureUserData::getType() { return m_type; }
FixtureUserData::~FixtureUserData() {}

//================ CarTireFUD =========================
CarTireFUD::CarTireFUD() : FixtureUserData(FUD_CAR_TIRE) {}

//================ GroundAreaFUD ======================
GroundAreaFUD::GroundAreaFUD(float fm, bool ooc)
    : FixtureUserData(FUD_GROUND_AREA), frictionModifier(fm), outOfCourse(ooc) {
}

//================ TDTire =============================
TDTire::TDTire(b2World* world) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    m_body = world->CreateBody(&bodyDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.5f, 1.25f);
    m_body->CreateFixture(&polygonShape, 1);

    m_body->SetUserData(this);
    m_currentTraction = 1;
}

TDTire::~TDTire() {
    m_body->GetWorld()->DestroyBody(m_body);
}

void TDTire::setCharacteristics(float f, float b, float d, float l) {
    m_maxForwardSpeed = f;
    m_maxBackwardSpeed = b;
    m_maxDriveForce = d;
    m_maxLateralImpulse = l;
}

void TDTire::addGroundArea(GroundAreaFUD* ga) {
    m_groundAreas.insert(ga);
    updateTraction();
}

void TDTire::removeGroundArea(GroundAreaFUD* ga) {
    m_groundAreas.erase(ga);
    updateTraction();
}

void TDTire::updateTraction() {
    if (m_groundAreas.empty()) {
        m_currentTraction = 1;
        return;
    }

    m_currentTraction = 0;
    for (auto ga : m_groundAreas)
        if (ga->frictionModifier > m_currentTraction)
            m_currentTraction = ga->frictionModifier;
}

b2Vec2 TDTire::getLateralVelocity() {
    b2Vec2 right = m_body->GetWorldVector(b2Vec2(1, 0));
    return b2Dot(right, m_body->GetLinearVelocity()) * right;
}

b2Vec2 TDTire::getForwardVelocity() {
    b2Vec2 forward = m_body->GetWorldVector(b2Vec2(0, 1));
    return b2Dot(forward, m_body->GetLinearVelocity()) * forward;
}

void TDTire::updateFriction() {
    b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
    if (impulse.Length() > m_maxLateralImpulse)
        impulse *= m_maxLateralImpulse / impulse.Length();

    m_body->ApplyLinearImpulse(m_currentTraction * impulse, m_body->GetWorldCenter(), true);
    m_body->ApplyAngularImpulse(m_currentTraction * 0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity(), true);

    b2Vec2 forward = getForwardVelocity();
    float speed = forward.Normalize();
    float drag = -2 * speed;
    m_body->ApplyForce(m_currentTraction * drag * forward, m_body->GetWorldCenter(), true);
}

void TDTire::updateDrive(int controlState) {
    float desiredSpeed = 0;

    switch (controlState & (TDC_UP | TDC_DOWN)) {
    case TDC_UP: desiredSpeed = m_maxForwardSpeed; break;
    case TDC_DOWN: desiredSpeed = m_maxBackwardSpeed; break;
    default: return;
    }

    b2Vec2 forward = m_body->GetWorldVector(b2Vec2(0, 1));
    float currentSpeed = b2Dot(getForwardVelocity(), forward);

    float force = 0;
    if (desiredSpeed > currentSpeed)
        force = m_maxDriveForce;
    else if (desiredSpeed < currentSpeed)
        force = -m_maxDriveForce;
    else
        return;

    m_body->ApplyForce(m_currentTraction * force * forward, m_body->GetWorldCenter(), true);
}

void TDTire::updateTurn(int controlState) {
    float torque = 0;

    switch (controlState & (TDC_LEFT | TDC_RIGHT)) {
    case TDC_LEFT: torque = 15; break;
    case TDC_RIGHT: torque = -15; break;
    default: break;
    }

    m_body->ApplyTorque(torque, true);
}

//================ TDCar ===============================
TDCar::TDCar(b2World* world) {
    b2BodyDef def;
    def.type = b2_dynamicBody;
    m_body = world->CreateBody(&def);
    m_body->SetAngularDamping(3);

    b2Vec2 vertices[8] = {
        {1.5f,0}, {3,2.5f}, {2.8f,5.5f}, {1,10},
        {-1,10}, {-2.8f,5.5f}, {-3,2.5f}, {-1.5f,0}
    };

    b2PolygonShape shape;
    shape.Set(vertices, 8);
    m_body->CreateFixture(&shape, 0.1f);

    b2RevoluteJointDef jointDef;
    jointDef.bodyA = m_body;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = 0;
    jointDef.upperAngle = 0;
    jointDef.localAnchorB.SetZero();

    float maxForwardSpeed = 250;
    float maxBackwardSpeed = -40;
    float backDrive = 300;
    float frontDrive = 500;
    float backLat = 8.5f;
    float frontLat = 7.5f;

    auto makeTire = [&](float x, float y, float drive, float lat, bool front) {
        TDTire* tire = new TDTire(world);
        tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, drive, lat);
        jointDef.bodyB = tire->m_body;
        jointDef.localAnchorA.Set(x, y);

        if (front) {
            if (!flJoint)
                flJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
            else
                frJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
        }
        else {
            world->CreateJoint(&jointDef);
        }

        m_tires.push_back(tire);
        };

    makeTire(-3, 0.75f, backDrive, backLat, false);
    makeTire(3, 0.75f, backDrive, backLat, false);
    flJoint = frJoint = nullptr;
    makeTire(-3, 8.5f, frontDrive, frontLat, true);
    makeTire(3, 8.5f, frontDrive, frontLat, true);
}

TDCar::~TDCar() {
    for (auto t : m_tires)
        delete t;
}

void TDCar::update(int controlState) {
    for (auto t : m_tires) t->updateFriction();
    for (auto t : m_tires) t->updateDrive(controlState);

    float lockAngle = 35 * DEGTORAD;
    float turnSpeed = 160 * DEGTORAD / 60.0f;

    float desired = 0;
    if (controlState & TDC_LEFT) desired = lockAngle;
    else if (controlState & TDC_RIGHT) desired = -lockAngle;

    float curr = flJoint->GetJointAngle();
    float delta = b2Clamp(desired - curr, -turnSpeed, turnSpeed);
    float newAngle = curr + delta;

    flJoint->SetLimits(newAngle, newAngle);
    frJoint->SetLimits(newAngle, newAngle);
}
*/