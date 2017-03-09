#include "mge/core/GameObject.hpp"
#include "DoorBehavior.h"
#include "Physics/RigidBody.h"


DoorBehavior::DoorBehavior() :AbstractBehaviour()
{
	//ctor?! : maybe
}

DoorBehavior::~DoorBehavior()
{
	//dtor?
}

void DoorBehavior::update(float pStep)
{
	//Comment
	if(_owner->getRigidBody() != nullptr)
	{
		btRigidBody* rb =_owner->getRigidBody()->GetRB();
		btTransform rbTrans = rb->getWorldTransform();
		btHingeConstraint* spDoorHinge;
		

	}
}

