#ifndef LookAt_H
#define LookAt_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

/**
 * Keeps the owner looking at the given target.
 */
class LookAt : public AbstractBehaviour
{
    public:
        LookAt (GameObject * pTarget,float pDistance);
        virtual ~LookAt();

        virtual void update(float pStep);

    private:

		void UpdatePosition();
        GameObject * _target;   //what are we looking at?
		glm::vec3 _posiition;
		glm::vec3 _targetPosition;

		float _distance;
};

#endif // LookAt_H
