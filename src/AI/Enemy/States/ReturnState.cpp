#include "ReturnState.h"
#include <ostream>
#include <iostream>
#include "AI/Enemy/EnemyAgent.h"
#include "AttackState.h"
#include "Physics/RigidBody.h"
#include "PatrolState.h"
#include "MgeVMath.h"

ReturnState::ReturnState(EnemyAgent* agent) : AbstractEnemyState(agent)
{
}

ReturnState::~ReturnState()
{
}

void ReturnState::Start()
{
	std::cout << "Starting " << typeid(*this).name() << "!" << std::endl;
}

void ReturnState::Stop()
{
	std::cout << "Stopping " << typeid(*this).name() << "!" << std::endl;
}

void ReturnState::Update(float deltaTime)
{
	if(_agent->GetSeesTarget())
	{
		_agent->SetState(typeid(AttackState));
	}
	else
	{
		glm::vec3 curVec = _agent->GetOwner()->getGameObject()->getLocalPosition();
		glm::vec3 tarVec;
		if (_agent->GetTargetNode()) tarVec = _agent->GetTargetNode()->getPosition();
		else tarVec = _agent->GetStartPos();

		glm::vec2 curXZ(curVec.x, curVec.z);
		glm::vec2 tarXZ(tarVec.x, tarVec.z);

		if (glm::distance(curXZ, tarXZ) > 1.0f)
		{
			GoToTarget(curVec, tarVec);
		}
		else
		{
			_agent->SetState(typeid(PatrolState));
		}
	}

}

void ReturnState::GoToTarget(glm::vec3 curVec, glm::vec3 tarVec)
{
	glm::vec3 difVec = tarVec - curVec;

	glm::quat curRot = _agent->GetOwner()->getGameObject()->getLocalRotation();
	glm::quat tarRot = MgeVMath::QuatLookRotation(glm::vec3(difVec.x, 0, difVec.z), _agent->GetOwner()->getGameObject()->getTransformUp());

	if (glm::angle(curRot, tarRot) > 0.05f)
	{
		glm::quat newRot = glm::slerp(curRot, tarRot, 0.05f);
		_agent->GetOwner()->getGameObject()->setLocalRotation(newRot);
	}

	_agent->GetOwner()->getGameObject()->translate(glm::vec3(0, 0, 0.2f));
}