#include "mge/core/GameObject.hpp"
#include <SFML/Window/Mouse.hpp>
#include "OrbCamera.h"
#include "glm.hpp"

OrbCamera::OrbCamera(float pDistance, float pMinTangle, float pMaxTangle, float pSpeed, GameObject *pTarget, sf::RenderWindow* pWindow) :
					AbstractBehaviour(), _target(pTarget), _window(pWindow), _distance(pDistance), _minTangle(pMinTangle), _maxTangle(pMaxTangle), _speed(pSpeed)
{
	//ctor
}

OrbCamera::~OrbCamera()
{
	//dtor
}



void OrbCamera::update(float pStep)
{
	//_owner->rotate(pStep * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f)); // rotates 45� per second
	_targetPosition = _target->getLocalPosition();
	UpdateMousePosition();
}

void OrbCamera::UpdateMousePosition()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button(0)))
	{
		_mousePos = glm::vec2(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y);

		if (_mousePos.x != 0 || _mousePos.y != 0)
		{
			rot.y += ((_mousePos.x - _window->getSize().x/2)*0.005f);
			rot.x += ((_mousePos.y - _window->getSize().y/2)*0.005f);
			if (rot.x < _minTangle) rot.x = _minTangle;
			if (rot.x > _maxTangle) rot.x = _maxTangle;//if rot.x reaches the max angle it will stay there
		}
	}

	//rotation matrix by y and x axis
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(rot.y * _speed), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(), glm::radians(rot.x*_speed), glm::vec3(1, 0, 0));

	glm::vec3 transform = glm::vec3(0, 0, _distance);

	glm::vec3 newPos = glm::vec3(rotation * glm::vec4(transform, 1.0f));

	_owner->setLocalPosition(newPos + _targetPosition);

	//Look at
	//normalize it that won't jump into the object
	//forward vector
	glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - _target->getLocalPosition());
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::normalize(glm::cross(forward, right));

	_owner->setTransform(
		glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1)));
}
