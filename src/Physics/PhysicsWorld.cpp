#include "PhysicsWorld.h"
#include "MgeRigidbody.h"

PhysicsWorld::PhysicsWorld()
{
	std::cout << std::boolalpha; //To make sure bools are printed as True/False 
	//Setting up Bullet Physics World
	std::cout << "Bullet Physics 3-2.86 is being initialized" << std::endl;
	_broadphase = new btDbvtBroadphase();
	std::cout << "Broadphase Loaded? "					<< (_broadphase != nullptr) << std::endl;
	_collisionConfiguration = new btDefaultCollisionConfiguration();
	std::cout << "Collision Configuration Loaded? "		<< (_collisionConfiguration != nullptr) << std::endl;
	_collisionDispatcher = new btCollisionDispatcher(_collisionConfiguration);
	std::cout << "Collision Dispatcher Loaded? "		<< (_collisionConfiguration != nullptr) << std::endl;
	_constraintSolver = new btSequentialImpulseConstraintSolver();
	std::cout << "Constraint Solver Loaded? "			<< (_constraintSolver != nullptr) << std::endl;
	_dynamicsWorld = new btDiscreteDynamicsWorld(_collisionDispatcher, _broadphase, _constraintSolver, _collisionConfiguration);
	std::cout << "Dynamics World Loaded? "				<< (_dynamicsWorld != nullptr) << std::endl;
	btVector3 gravity = btVector3(0, -10, 0);
	_dynamicsWorld->setGravity(gravity);
	std::cout << "Dynamics World Initialized with a Gravity of " << gravity.getX() << "," << gravity.getY() << "," << gravity.getZ() << "." << std::endl << std::endl;
	std::cout << std::noboolalpha;
}

btDiscreteDynamicsWorld* PhysicsWorld::GetDynamicsWorld() const
{
	return _dynamicsWorld;
}

void PhysicsWorld::_checkCollisions()
{
	int numManifolds = _collisionDispatcher->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = _collisionDispatcher->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();
		if (obA != nullptr && obB != nullptr)
		{
			_triggerOnCollision(obA, obB);
			_triggerOnCollision(obB, obA);
		}
	}
}

void PhysicsWorld::_triggerOnCollision(const btCollisionObject* ob1, const btCollisionObject* ob2)
{
	if (!ob1->isActive() || !ob2->isActive()) return;
	//if (ob1->isStaticOrKinematicObject() || ob1->isStaticOrKinematicObject()) return;
	if (const MgeRigidbody* rb = dynamic_cast<const MgeRigidbody *>(ob1))
	{
		rb->getGameObject()->OnCollision(ob2);
	}
}

PhysicsWorld::~PhysicsWorld()
{
	delete _dynamicsWorld;
	delete _constraintSolver;
	delete _collisionDispatcher;
	delete _collisionConfiguration;
	delete _broadphase;
}

void PhysicsWorld::SimulateWorld(float deltaTime)
{
	_dynamicsWorld->stepSimulation(deltaTime);
	_checkCollisions();
}
