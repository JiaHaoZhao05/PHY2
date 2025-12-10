#include <vector>
#include <set>
#include "box2d/box2d.h"
#include "PhysicEntity.h"

class Car : public PhysicEntity{
public:
    Car(PhysBody* _body, Module* _listener) {
        physBody = _body;
        physBody->listener = _listener;
    }

    ~Car();

    void update();

private:
    b2RevoluteJoint* flJoint;
    b2RevoluteJoint* frJoint;
};