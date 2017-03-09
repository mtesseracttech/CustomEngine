#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "MovementBehaviour.h"

MovementBehaviour::MovementBehaviour(sf::RenderWindow* pWindow, float forwardSpeed, float sideSpeed) : AbstractBehaviour(), _window(pWindow), _forwardSpeed(forwardSpeed), _sideSpeed(sideSpeed)
{
}

MovementBehaviour::~MovementBehaviour()
{
}

void MovementBehaviour::update(float pStep)
{
	UpdateMovement(pStep);
}

void MovementBehaviour::UpdateMovement(float pStep)
{
	float forwardSpeed = 0.0f;
	float sideSpeed = 0.0f;

	if (_owner->getRigidBody() != nullptr)
	{
		btRigidBody* rb = _owner->getRigidBody()->GetRB();
		btTransform rbTrans = rb->getWorldTransform();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			forwardSpeed = -_forwardSpeed;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			forwardSpeed = _forwardSpeed;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			sideSpeed = _sideSpeed;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			sideSpeed = -_sideSpeed;
		}
		else
		{
			sideSpeed = 0;
			forwardSpeed = 0;
		}

		rb->applyForce(btVector3(sideSpeed, 0, forwardSpeed), btVector3(0, 0, 0));

		//ROTATION
		btVector3 mousePosit = btVector3(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y,0);
		btScalar Dx = mousePosit.getX() - _window->getSize().x / 2;
		btScalar Dz = mousePosit.getY() - _window->getSize().y / 2;
		btScalar angleinradians = btAtan2Fast(Dx, Dz);
		btScalar angleInDegrees = btDegrees(angleinradians);

		btQuaternion quat = rbTrans.getRotation();
		quat.setRotation(btVector3(0, 1, 0), angleinradians);
		rbTrans.setRotation(quat);
		rb->setWorldTransform(rbTrans);
	}
}
