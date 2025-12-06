#include <vector>
#include <set>
#include "box2d/box2d.h"

class Car {
public:
    Car();
    ~Car();

    void update();

private:
    b2Body* m_body;
    b2RevoluteJoint* flJoint;
    b2RevoluteJoint* frJoint;
};