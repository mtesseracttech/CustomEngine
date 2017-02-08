#pragma once
#include "mge/core/AbstractGame.hpp"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class DebugHud;

class PhysicsGame : public AbstractGame
{
	public:
		PhysicsGame();
		~PhysicsGame();
		virtual void initialize();
		void initializeBullet();
	protected:
		virtual void _initializeScene();
		virtual void _render();
	private:
		DebugHud* _hud;
		void _updateHud();
		PhysicsGame(const PhysicsGame&);
		PhysicsGame& operator=(const PhysicsGame&);
		//Bullet Vars
		btBroadphaseInterface *					_broadphase;
		btDefaultCollisionConfiguration *		_collisionConfiguration;
		btCollisionDispatcher *					_dispatcher;
		btSequentialImpulseConstraintSolver *	_solver;
		btDiscreteDynamicsWorld *				_dynamicsWorld;
};

