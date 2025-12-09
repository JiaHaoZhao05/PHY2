#include "Car.h"

Car::Car() {}

Car::Car(PhysBody* _body, Module* _listener) {
	physBody = _body;
	physBody->listener = listener;
}

Car::~Car() {}

void Car::update() {}