#ifndef MOVEMENTBEHAVIOR_H
#define MOVEMENTBEHAVIOR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include <SFML/Graphics/RenderWindow.hpp>


/**
* KeysBehaviour allows you to move an object using the keyboard in its own local space.
* Left right turns, forward back moves.
*/
class MovementBehaviour : public AbstractBehaviour
{
public:
	MovementBehaviour(sf::RenderWindow* pWindow, float pMoveSpeedZ = 10, float pMoveSpeedX = 10);
	virtual ~MovementBehaviour();
	void update(float pStep) override;

private:
	sf::RenderWindow* _window;
	float _forwardSpeed;
	float _sideSpeed;
	void UpdateMovement(float pStep);

};

#endif MOVEMENTBEHAVIOR_H
