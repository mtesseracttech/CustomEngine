#include "mge/core/GameObject.hpp"
#include "OrbitBehavior.h"
#include <SFML/Window/Mouse.hpp>


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
	glm::vec3 transform;
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button(1)))
	{
		transform = glm::vec3(0, _distance + 10, 0);//figure out why 1
	}
	else
	{
		transform = glm::vec3(0, _distance, 0);//figure out why 1
	}

	_owner->setLocalPosition(transform + _targetPosition);
}
