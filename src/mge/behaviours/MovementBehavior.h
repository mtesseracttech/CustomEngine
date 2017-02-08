#ifndef MOVEMENTBEHAVIOR_H
#define MOVEMENTBEHAVIOR_H

#include "mge/behaviours/AbstractBehaviour.hpp"


class MovementBehavior : public AbstractBehaviour
{
public:
	MovementBehavior(float pMoveSpeedZ = 10, float pMoveSpeedX = 10, float pTurn = 200);
	virtual ~MovementBehavior();
	virtual void update(float pStep);

private:
	void UpdateMovement(float pStep);
	void UpdateRotation();
	glm::vec2 rot;	
	glm::vec2 _mousePos;

	float _moveSpeedZ;
	float _turnSpeed;
	float _moveSpeedX;
	
};

#endif //MOVEMENTBEHAVIOR_H
