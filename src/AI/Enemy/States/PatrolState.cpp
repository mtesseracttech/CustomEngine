#include "PatrolState.h"
#include <ostream>
#include <iostream>
#include "AI/Navigation//NavigationPath.h"
#include "AI/Enemy/EnemyAgent.h"
#include "MgeVMath.h"
#include "AttackState.h"

PatrolState::PatrolState(EnemyAgent* agent) : AbstractEnemyState(agent), _path(nullptr), _pathMode(false), _alignedAtStart(false)
{
}

PatrolState::~PatrolState(){}

void PatrolState::Start()
{
	std::cout << "Starting " << typeid(*this).name() << "!" << std::endl;
	if(!_agent->GetTargetNode())
	{
		if(_path && _path->GetNodeCount() > 0)
		{
			std::cout << "Setting up the starting node!" << std::endl;
			_agent->SetTargetNode(GetClosestNode());
			_pathMode = true;
		}
		else
		{
			_pathMode = false;
		}
	}
	_alignedAtStart = false;
	_speed = 0.0f;
}

void PatrolState::Stop()
{
	std::cout << "Stopping " << typeid(*this).name() << "!" << std::endl;
}

void PatrolState::Update(float deltaTime)
{
	if (_agent->GetSeesTarget())
	{
		_agent->SetState(typeid(AttackState));
	}
	else
	{
		if(!_alignedAtStart)
		{
			_agent->GetOwner()->getGameObject()->setLocalRotation(glm::slerp(_agent->GetOwner()->getGameObject()->getLocalRotation(), _agent->GetStartRot(), 0.1f));
			if (glm::angle(_agent->GetOwner()->getGameObject()->getLocalRotation(), _agent->GetStartRot()) > 0.01f) _alignedAtStart = true;
		}
		else
		{
			if (_path) FollowPath();
			else RotateAroundAxis();
		}
	}
}

NavigationNode* PatrolState::GetClosestNode() const
{
	glm::vec3 ownerPos = MgeVMath::btToGlmVec(_agent->GetOwner()->getWorldTransform().getOrigin());
	float lowestDist = std::numeric_limits<float>::infinity();
	NavigationNode * closestNode = new NavigationNode(glm::vec3(0,0,0));
	
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

void PatrolState::RotateAroundAxis() const
{
	_agent->GetOwner()->getGameObject()->rotate(IdleRotationSpeed, glm::vec3(0, 1, 0));
}

void PatrolState::SetPath(NavigationPath* path)
{
	_path = path;
	_agent->SetTargetNode(GetClosestNode());
	_pathMode = true;
}

void PatrolState::FollowPath()
{
	glm::vec3 curVec = _agent->GetOwner()->getGameObject()->getLocalPosition();
	glm::vec3 tarVec = _agent->GetTargetNode()->getPosition();//_targetNode->getPosition();
	glm::vec3 difVec = tarVec - curVec;

	glm::quat curRot = _agent->GetOwner()->getGameObject()->getLocalRotation();
	glm::quat tarRot = MgeVMath::QuatLookRotation(glm::vec3(difVec.x, 0, difVec.z), _agent->GetOwner()->getGameObject()->getTransformUp());
	
	if(glm::angle(curRot, tarRot) > 0.01f)
	{
		glm::quat newRot = glm::slerp(curRot, tarRot, SlerpSpeed);
		_agent->GetOwner()->getGameObject()->setLocalRotation(newRot);
	}
	
	if(_speed < MaxSpeed)
	{
		std::cout << "Speed: " << _speed << std::endl;
		_speed += MaxSpeed / 60;
	}
	else
	{
		_speed = MaxSpeed;
	}

	_agent->GetOwner()->getGameObject()->translate(glm::vec3(0,0,_speed));
	glm::vec2 curVec2(curVec.x, curVec.z), tarVec2(tarVec.x, tarVec.z);
	if (glm::distance(curVec2, tarVec2) <= 3)
	{
		_agent->SetCurrentNode(_agent->GetTargetNode());
		_agent->SetTargetNode(_agent->GetCurrentNode()->getRandomNeighbour());
		//_currentNode = _targetNode;
		//_targetNode = _currentNode->getRandomNeighbour();
	}
}
