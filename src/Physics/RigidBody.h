#pragma once
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "mge/core/GameObject.hpp"
#include "MgeRigidbody.h"

class RigidBody
{
	public:
		RigidBody(GameObject* owner, btCollisionShape* shape, btDefaultMotionState* motionState, btDynamicsWorld* world);
		RigidBody(GameObject* owner, btCollisionShape* shape, btDefaultMotionState* motionState, float mass, btDynamicsWorld* world);
		~RigidBody();
		void					AddToWorld(btDynamicsWorld* world);
		void					RemoveFromWorld();
		btMotionState*			GetMotionState()						const;
		GameObject*				GetOwner()								const;
		MgeRigidbody*			GetRB()									const;
		btDynamicsWorld*		GetDynamicsWorld()						const;
private:
		btCollisionShape*		_shape;
		btDefaultMotionState*	_defMState;
		MgeRigidbody*			_rigidBody;
		btDynamicsWorld*		_world;
		GameObject*				_owner;
};

