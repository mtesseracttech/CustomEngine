#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "MovementBehavior.h"
#include <SFML/Window/Mouse.hpp>
# define M_PI          3.141592653589793238462643383279502884L /* pi */
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
	//get mouse position
	_mousePos = glm::vec2(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

	glm::mat4 translationMatrix;
	glm::mat4 myIdentityMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix;

	float moveSpeedZ = 0.0f; //default if no keys
	float turnSpeed = 0.0f;
	float moveSpeedX = 0.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		moveSpeedZ = -_moveSpeedZ;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		moveSpeedZ = _moveSpeedZ;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		moveSpeedX =  _moveSpeedX;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		moveSpeedX = -_moveSpeedX;
	}

	glm::vec4 transform = glm::vec4(moveSpeedX*pStep, 0, moveSpeedZ*pStep,1);

	glm::vec3 myRotationAxis = glm::vec3(0, 1, 0);

	//TransformedVector = TranslationMatrix * RotationMatrix *  OriginalVector;
	//glm::vec3 forward = glm::normalize(_owner->getTransform()[2]);
	/**
	float dx = _mousePos.x - _owner->getLocalPosition().x;
	float dy = _mousePos.y - _owner->getLocalPosition().y;
	float angleinradians = std::atan2(dy, dx);
	float angleindegrees = angleinradians * 180 / M_PI;
	/**/

	
	glm::vec4 transformedVector = translationMatrix * transform;

	_owner->translate(transformedVector);

	
}


