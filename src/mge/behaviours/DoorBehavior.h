#ifndef DOORBEHAVIOR_H
#define DOORBEHAVIOR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <glm/glm.hpp>


class DoorBehavior : public AbstractBehaviour
{
public:
	DoorBehavior();
	virtual ~DoorBehavior();
	virtual void update(float pStep);
private:


};


#endif DOORBEHAVIOR_H

