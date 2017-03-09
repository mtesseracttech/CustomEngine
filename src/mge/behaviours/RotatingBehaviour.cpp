#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/core/GameObject.hpp"

RotatingBehaviour::RotatingBehaviour(float pSpeed, float pAngle) :AbstractBehaviour(),_speed(pSpeed),_angle(pAngle)
{
	//ctor
}

RotatingBehaviour::~RotatingBehaviour()
{
	//dtor
}

void RotatingBehaviour::update(float pStep)
{
	_owner->rotate(_speed * glm::radians(_angle), glm::vec3( 0.0f, 1.0f, 0.0f ) ); // rotates 45° per second
}
