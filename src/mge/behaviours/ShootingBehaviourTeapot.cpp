#include "ShootingBehaviourTeapot.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Physics/RigidBody.h>
#include <Physics/MgeRigidbody.h>
#include <mge/config.hpp>
#include <mge/core/Mesh.hpp>
#include "mge/materials/ColorMaterial.hpp"
#include "Physics/PhysicsWorldGame.h"

using namespace glm;
using namespace std;

ShootingBehaviourTeapot::ShootingBehaviourTeapot(PhysicsWorld* phyWorld) : _dynWorld(nullptr)
{
	_gizmoMesh		= Mesh::load(config::MGE_MODEL_PATH + "Axis_Sphere.obj");
	_gizmoMaterial	= new ColorMaterial(vec3(0.5f, 0.5f, 0.5f));
	_physicsWorld	= phyWorld;
}

ShootingBehaviourTeapot::~ShootingBehaviourTeapot()
{
	delete _gizmoMesh;
	delete _gizmoMaterial;
	for (GameObject* gizmo : _gizmoList)
	{
		delete gizmo;
	}
	_gizmoList.clear();
}

void ShootingBehaviourTeapot::SpawnGizmo(glm::vec3 point)
{
	cout << "Spawn Gizmo!" << endl;
	GameObject* gizmo = new GameObject("gizmo", point);
	gizmo->scale(vec3(0.2, 0.2, 0.2));
	gizmo->setMesh(_gizmoMesh);
	gizmo->setMaterial(_gizmoMaterial);
	_physicsWorld->add(gizmo);
	_gizmoList.push_back(gizmo);
}


void ShootingBehaviourTeapot::update(float pStep)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		cout << "SHOT FIRED!" << endl;
		vec3 fwd	= _owner->getTransformForward();
		vec3 start	= _owner->getLocalPosition();
		vec3 end	= start + (fwd * 10.0f);

		btVector3 btStart(start.x, start.y, start.z);
		btVector3 btEnd  (end.x	 , end.y  , end.z  );

		btCollisionWorld::ClosestRayResultCallback callback(btStart, btEnd);

		_dynWorld->rayTest(btStart, btEnd, callback);

		if (callback.hasHit()) 
		{
			cout << "Got a hit!" << endl;
			btEnd				= callback.m_hitPointWorld;
			btVector3 normal	= callback.m_hitNormalWorld;
			SpawnGizmo(vec3(btEnd.getX(), btEnd.getY(), btEnd.getZ()));
			const btCollisionObject * colObj	= callback.m_collisionObject;
			if (const MgeRigidbody* rb = dynamic_cast<const MgeRigidbody*>(colObj))
			{
				if (rb->isActive() && !rb->isStaticOrKinematicObject())
				{
					const float force = 100;
					btVector3 forceVector(fwd.x * force, fwd.y * force, fwd.z * force);
					cout << "BLASTING THAT SHIT AWAY" << endl;
					rb->getMovableRigidBody()->applyForce(forceVector, btEnd);
				}
			}
		}
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
	{
		for (GameObject* gizmo : _gizmoList)
		{
			delete gizmo;
		}
		_gizmoList.clear();
	}
}

void ShootingBehaviourTeapot::setOwner(GameObject* pGameObject)
{
	AbstractBehaviour::setOwner(pGameObject);
	_dynWorld = _owner->getRigidBody()->GetDynamicsWorld();
}
