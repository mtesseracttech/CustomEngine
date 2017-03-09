#pragma once
#include "mge\behaviours\AbstractBehaviour.hpp"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"

class InteractionBehaviour : public AbstractBehaviour
{
	public:
		InteractionBehaviour(btCollisionWorld* world, GameObject* owner);
		~InteractionBehaviour();
		void update(float pStep) override;
	GameObject* RaycastInFront(float length);

private:
		btCollisionWorld*		_world;
		GameObject*				_owner;
};

