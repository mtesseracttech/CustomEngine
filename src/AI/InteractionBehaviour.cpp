#include "InteractionBehaviour.h"
#include <iostream>
#include "Physics/MgeRigidbody.h"
#include "Physics/Rigidbody.h"
#include <glm/gtx/matrix_decompose.hpp>

InteractionBehaviour::InteractionBehaviour(btCollisionWorld* world, GameObject* owner) : _world(nullptr), _owner(nullptr)
{
}

InteractionBehaviour::~InteractionBehaviour()
{
}

void InteractionBehaviour::update(float pStep)
{
}

GameObject* InteractionBehaviour::RaycastInFront(float length)
{
	/**
	//glm::quat ownerRot = owner
	//btVector3 start(ownerTrans.x, ownerTrans.y, ownerTrans.z), end(0, 0, 0);
	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
	// Perform raycast
	std::cout << "Performing Raycast!" << std::endl;
	_world->rayTest(start, end, RayCallback);

	if (RayCallback.hasHit()) {
		std::cout << "got a hit!" << std::endl;
		end = RayCallback.m_hitPointWorld;
		const btCollisionObject* hit = RayCallback.m_collisionObject;

		if (const MgeRigidbody* rb = dynamic_cast<const MgeRigidbody*>(hit))
		{
			std::cout << rb->getGameObject()->getName() << " was detected!" << std::endl;
		}
		else
		{
			std::cout << "MEEH FUCK" << std::endl;
		}

		// Do some clever stuff here
	}
	else
	{
		std::cout << "no hit was detected!" << std::endl;
	}
	/**/
}