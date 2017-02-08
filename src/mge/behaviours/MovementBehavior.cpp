#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "MovementBehavior.h"
#include <SFML/Window/Mouse.hpp>
MovementBehavior::MovementBehavior(float pMoveSpeedZ, float pMoveSpeedX, float pTurn) : AbstractBehaviour(), _moveSpeedZ(pMoveSpeedZ), _moveSpeedX(pMoveSpeedX), _turnSpeed(pTurn)
{
}

MovementBehavior::~MovementBehavior()
{}

void MovementBehavior::update(float pStep)
{
	UpdateMovement(pStep);
}

void MovementBehavior::UpdateMovement(float pStep)
{
	float moveSpeedZ = 0.0f; //default if no keys
	float turnSpeed = 0.0f;
	float moveSpeedX = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		moveSpeedZ = _moveSpeedZ;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		moveSpeedZ = -_moveSpeedZ;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		moveSpeedX =- _moveSpeedX;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		moveSpeedX = _moveSpeedX;
	}
	//get mouse position
	_mousePos = glm::vec2(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

	//get angle between player and mouse. set it to radians

	glm::vec3 forward = glm::normalize(_owner->getTransform()[2]);
	glm::vec3 _mouseXY = glm::normalize( glm::vec3(_mousePos.x, 0, _mousePos.y));
	float angle = glm::normalizeDot(forward, _mouseXY);
	//rotate object with angle
	_owner->rotate(angle , glm::vec3(0, 1, 0));

	//translate the object in its own local space
	_owner->translate(glm::vec3(moveSpeedX*pStep, 0.0f, moveSpeedZ*pStep));

	
}


