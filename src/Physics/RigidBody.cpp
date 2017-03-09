#include "RigidBody.h"
#include <iostream>
#include "GameObjectMotionState.h"

//Constructor for static rigidbodies (level, walls, floors, static props, etc.)
RigidBody::RigidBody(GameObject* owner, btCollisionShape* shape, btDefaultMotionState* motionState, btDynamicsWorld* world): _shape(nullptr), _defMState(nullptr)
{
	_owner = owner;
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motionState, shape, btVector3(0, 0, 0));
	_rigidBody = new MgeRigidbody(rigidBodyCI, owner);
	_world = world;
	std::cout << "Adding static rigidbody to world!" << std::endl;
	_world->addRigidBody(_rigidBody);
}

//Constructor for dynamic rigidbodies (props, player, enemies, etc.)
RigidBody::RigidBody(GameObject* owner, btCollisionShape* shape, btDefaultMotionState* motionState, float mass, btDynamicsWorld* world)
{
	_owner = owner;
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, shape, inertia);
	_shape = shape;
	_defMState = motionState;
	_rigidBody = new MgeRigidbody(rigidBodyCI, owner);
	_rigidBody->setActivationState(DISABLE_DEACTIVATION);
	_world = world;
	std::cout << "Adding rigidbody to world!" << std::endl;
	_world->addRigidBody(_rigidBody);
}

RigidBody::~RigidBody()
{
	if (_world != nullptr)_world->removeRigidBody(_rigidBody);
	delete _rigidBody->getMotionState();
	delete _rigidBody;
	delete _shape;
}

void RigidBody::AddToWorld(btDynamicsWorld* world)
{
	if(_world == nullptr)
	{
		_world = world;
		_world->addRigidBody(_rigidBody);

	}
	else
	{
		std::cout << "The Rigidbody was already added to a dynamics world" << std::endl;
	}
}

void RigidBody::RemoveFromWorld()
{
	if(_world != nullptr)
	{
		_world->removeRigidBody(_rigidBody);
		_world = nullptr;
	}
	else
	{
		std::cout << "The rigidbody is already removed from the dynamics world" << std::endl;
	}
}

btMotionState* RigidBody::GetMotionState() const
{
	return _rigidBody->getMotionState();
}

GameObject* RigidBody::GetOwner() const
{
	return _owner;
}

MgeRigidbody* RigidBody::GetRB() const
{
	return _rigidBody;
}

btDynamicsWorld* RigidBody::GetDynamicsWorld() const
{
	return _world;
}
