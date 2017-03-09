#ifndef ROTATINGBEHAVIOUR_H
#define ROTATINGBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"

/**
 * Simply rotates the object around its origin
 */
class RotatingBehaviour : public AbstractBehaviour
{
	public:
		RotatingBehaviour(float pSpeed, float pAngle);
		virtual ~RotatingBehaviour();
		void update(float pStep) override;

	private:
		float _speed;
		float _angle;
};

#endif // ROTATINGBEHAVIOUR_H
