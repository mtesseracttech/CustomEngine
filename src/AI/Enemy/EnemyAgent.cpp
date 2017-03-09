#define _USE_MATH_DEFINES
#include "EnemyAgent.h"
#include <cmath>
#include <iostream>
#include "States/PatrolState.h"
#include "States/ChaseState.h"
#include "States/AttackState.h"
#include "States/LookoutState.h"
#include "States/ReturnState.h"
#include "Physics/RigidBody.h"
#include "glm/gtx/vector_angle.hpp"
#include "MgeVMath.h"

using namespace glm;

EnemyAgent::EnemyAgent(btDynamicsWorld* dynWorld): _currentState(nullptr), _path(nullptr)
{
	_dynWorld = dynWorld;

	_stateCache[typeid(PatrolState)	] = new PatrolState	(this);
	_stateCache[typeid(ChaseState)	] = new ChaseState	(this);
	_stateCache[typeid(AttackState)	] = new AttackState	(this, _dynWorld);
	_stateCache[typeid(LookoutState)] = new LookoutState(this);
	_stateCache[typeid(ReturnState)	] = new ReturnState	(this);

	//SetState(typeid(AttackState));
	SetState(typeid(PatrolState));

	std::cout << "State Cache Size: " << _stateCache.size() << std::endl;
	std::cout << "Current State: " << typeid(_currentState).name() << std::endl;
}

EnemyAgent::~EnemyAgent()
{
}

void EnemyAgent::SetState(const type_info &stateType)
{
	std::cout << "Switching state to: " << stateType.name() << std::endl;
	AbstractEnemyState* newState = _stateCache.find(stateType)->second;
	if(newState != nullptr)
	{
		std::cout << "Stopping old State!" << std::endl;
		if(_currentState != nullptr) _currentState->Stop();
		_currentState = newState;
		std::cout << "Starting new State" << std::endl;
		_currentState->Start();
	}
	else
	{
		std::cout << "The chosen state could not be switched to!" << std::endl;
	}
}

AbstractEnemyState* EnemyAgent::GetState() const
{
	return _currentState;
}

void EnemyAgent::update(float pStep)
{
	_seesTarget = SeesTarget();
	_currentState->Update(pStep);
}

bool EnemyAgent::SeesTarget() const
{
	btVector3 ownerBtPos	= _ownerRB->getWorldTransform().getOrigin();
	btVector3 targetBtPos	= _targetRB->getWorldTransform().getOrigin();

	vec3 ownerPos			= MgeVMath::btToGlmVec(ownerBtPos);//toGlmVector(ownerBtPos);
	vec3 targetPos			= MgeVMath::btToGlmVec(targetBtPos);//toGlmVector(targetBtPos);
	vec3 diffVec			= targetPos - ownerPos;

	if(length(diffVec) <= _sightRange)
	{
		float targetAngle = angle(normalize(diffVec), _owner->getTransformForward()) * (180 / (M_PI));

		if (targetAngle > _sightConeAngle / 2)
		{
			return false;
		}

		btCollisionWorld::ClosestRayResultCallback callback(ownerBtPos, targetBtPos);
		_dynWorld->rayTest(ownerBtPos, targetBtPos, callback);
		if(callback.hasHit())
		{
			if (const MgeRigidbody* rb = dynamic_cast<const MgeRigidbody*>(callback.m_collisionObject))
			{
				if (rb->getGameObject() == _targetRB->getGameObject()) return true;
			}
		}
	}
	return false;
}

MgeRigidbody* EnemyAgent::GetTarget() const
{
	return _targetRB;
}

MgeRigidbody* EnemyAgent::GetOwner() const
{
	return _ownerRB;
}

void EnemyAgent::SetTarget(GameObject* target)
{
	_targetRB = target->getRigidBody()->GetRB();
}

void EnemyAgent::SetTarget(MgeRigidbody* target)
{
	_targetRB = target;
}

void EnemyAgent::setOwner(GameObject* pGameObject)
{
	AbstractBehaviour::setOwner(pGameObject);
	_ownerRigidBody = _owner->getRigidBody();
	_ownerRB		= _ownerRigidBody->GetRB();
	_startPos		= _owner->getWorldPosition();
	_startRot		= _owner->getWorldRotation();
}

bool EnemyAgent::GetSeesTarget() const
{
	return _seesTarget;
}

btDynamicsWorld* EnemyAgent::GetDynWorld() const
{
	return _dynWorld;
}

void EnemyAgent::SetNavigationPath(NavigationPath* path)
{
	_path = path;
}

glm::vec3 EnemyAgent::GetStartPos() const
{
	return _startPos;
}

glm::quat EnemyAgent::GetStartRot() const
{
	return _startRot;
}

void EnemyAgent::SetPath(NavigationPath* path)
{
	_path = path;
	PatrolState * state = static_cast<PatrolState*>(_stateCache[typeid(PatrolState)]);
	_currentNode = GetClosestNode();
	_targetNode = GetClosestNode();
	state->SetPath(path);
	
}

NavigationNode* EnemyAgent::GetClosestNode() const
{
	glm::vec3 ownerPos = MgeVMath::btToGlmVec(GetOwner()->getWorldTransform().getOrigin());
	float lowestDist = std::numeric_limits<float>::infinity();
	NavigationNode * closestNode = new NavigationNode(glm::vec3(0, 0, 0));

	for (int i = 0; i < _path->GetNodeCount(); i++)
	{
		NavigationNode * node = _path->GetNodeAt(i);
		float dist = glm::distance2(node->getPosition(), ownerPos);
		if (dist < lowestDist)
		{
			lowestDist = dist;
			closestNode = node;
		}
	}
	return closestNode;
}

NavigationNode* EnemyAgent::GetTargetNode() const
{
	return _targetNode;
}

NavigationNode* EnemyAgent::GetCurrentNode() const
{
	return _currentNode;
}

void EnemyAgent::SetTargetNode(NavigationNode* node)
{
	_targetNode = node;
}

void EnemyAgent::SetCurrentNode(NavigationNode* node)
{
	_currentNode = node;
}
