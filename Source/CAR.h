#include <vector>
#include <set>
#include "box2d/box2d.h"
#include "PhysicEntity.h"

class Car : PhysicEntity{
public:
    Car();
    Car(PhysBody* _body, Module* _listener);
    ~Car();

    void update();

private:
    b2Body* m_body;
    b2RevoluteJoint* flJoint;
    b2RevoluteJoint* frJoint;
};