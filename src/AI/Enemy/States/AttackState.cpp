#include "AttackState.h"
#include <ostream>
#include <iostream>
#include "AI/Enemy/EnemyAgent.h"
#include "ChaseState.h"
#include "glm.hpp"
#include "MgeVMath.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "PatrolState.h"
#include "ReturnState.h"

//using namespace glm;

AttackState::AttackState(EnemyAgent* agent, btDynamicsWorld* dynWorld) : AbstractEnemyState(agent)
{
	_dynWorld = dynWorld;
}

AttackState::~AttackState()
{
}

void AttackState::Start()
{
	std::cout << "Starting " << typeid(*this).name() << "!" << std::endl;
}

void AttackState::Update(float deltaTime)
{
	if(_agent->GetSeesTarget())
	{
		RotateToTarget();
		LineOfSightHandler(deltaTime);
	}
	else
	{
		//_agent->SetState(typeid(PatrolState));
		_agent->SetState(typeid(ReturnState));
	}
}

void AttackState::Stop()
{
	std::cout << "Stopping " << typeid(*this).name() << "!" << std::endl;
}

void AttackState::RotateToTarget()
{
	glm::quat oriRot = _agent->GetOwner()->getGameObject()->getLocalRotation();

	glm::vec3 tarPos = MgeVMath::btToGlmVec(_agent->GetTarget()->getWorldTransform().getOrigin());
	glm::vec3 ownPos = MgeVMath::btToGlmVec(_agent->GetOwner()->getWorldTransform().getOrigin());

	glm::vec3 difVec = tarPos-ownPos;
	//glm::quat difRot = MgeVMath::QuatLookRotation(difVec, _agent->GetOwner()->getGameObject()->getTransformUp()); // also rotates up/down to face player
	glm::quat difRot = MgeVMath::QuatLookRotation(glm::vec3(difVec.x, 0,difVec.z), _agent->GetOwner()->getGameObject()->getTransformUp()); //aims to direction of player
	glm::quat newRot = glm::slerp(oriRot, difRot, _slerpSpeed);
	_agent->GetOwner()->getGameObject()->setLocalRotation(newRot);
}

void AttackState::LineOfSightHandler(float deltaTime)
{
	glm::vec3 turVec = MgeVMath::btToGlmVec(_agent->GetOwner()->getWorldTransform().getOrigin());
	glm::vec3 difVec = MgeVMath::btToGlmVec(_agent->GetTarget()->getWorldTransform().getOrigin()) - turVec;
	glm::vec3 customVec = _agent->GetOwner()->getGameObject()->getTransformForward(); //Customvec is only useful when a different casting position (turret) is used!
	customVec = glm::normalize(customVec);
	customVec = customVec * glm::length(difVec);
	customVec.y = difVec.y;

	glm::vec3 aimVec = customVec; //temp, to make switching out for more realistic aiming easier

	btVector3 btTurVec = MgeVMath::glmToBtVec(turVec);
	btVector3 btAimVec = MgeVMath::glmToBtVec(aimVec);
	btVector3 btAimVecEnd = btTurVec + btAimVec;

	btCollisionWorld::ClosestRayResultCallback callback(btTurVec, btAimVecEnd);
	_dynWorld->rayTest(btTurVec, btAimVecEnd, callback);
	if(callback.hasHit())
	{
		std::cout << "Aiming at the player!" << std::endl;
		Shoot(callback, deltaTime);
	}
}


void AttackState::Shoot(const btCollisionWorld::ClosestRayResultCallback& callback, float deltaTime)
{
	std::cout << "Shoot Activated" << std::endl;
	const btCollisionObject * colObj = callback.m_collisionObject;
	if (const MgeRigidbody* rb = dynamic_cast<const MgeRigidbody*>(colObj))
	{
		std::cout << "Shooting " << rb->getGameObject()->getName() << std::endl;
	}
}