#pragma once
#include "mge\core\World.hpp"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

class PhysicsWorld : public World
{
	public:
		PhysicsWorld();
		~PhysicsWorld();
		void SimulateWorld(float deltaTime);
		btDiscreteDynamicsWorld*			GetDynamicsWorld() const;
	private:
		btBroadphaseInterface*				 _broadphase;
		btDefaultCollisionConfiguration*	 _collisionConfiguration;
		btCollisionDispatcher*				 _collisionDispatcher;
		btSequentialImpulseConstraintSolver* _constraintSolver;
		btDiscreteDynamicsWorld*			 _dynamicsWorld;
		void _checkCollisions();
		void _triggerOnCollision(const btCollisionObject* ob1, const btCollisionObject* ob2);
};

