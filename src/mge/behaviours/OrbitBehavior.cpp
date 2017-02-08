#include "mge/core/GameObject.hpp"
#include "OrbitBehavior.h"


OrbitBehavior::OrbitBehavior(float pDistance, GameObject *pTarget) :AbstractBehaviour(), _target(pTarget), _distance(pDistance)
{
	//ctor
}

OrbitBehavior::~OrbitBehavior()
{
	//dtor
}



void OrbitBehavior::update(float pStep)
{
	//_owner->rotate(pStep * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f)); // rotates 45° per second
	_targetPosition = _target->getLocalPosition();
	UpdateMousePosition();
}

void OrbitBehavior::UpdateMousePosition()
{

	glm::vec3 transform = glm::vec3(0, _distance, 1);//figure out why 1
	_owner->setLocalPosition(transform + _targetPosition);

	//forward vector 
	glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - _target->getLocalPosition());
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::normalize(glm::cross(forward, right));

	_owner->setTransform(
		glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1)));
}
