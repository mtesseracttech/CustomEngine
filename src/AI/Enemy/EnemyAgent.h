#pragma once
#include <map>
#include "States/AbstractEnemyState.h"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <typeindex>
#include "mge/core/GameObject.hpp"
#include "Physics/MgeRigidbody.h"
#include "AI/Navigation/NavigationPath.h"
#include "glm.hpp"

class EnemyAgent : public AbstractBehaviour
{
public:
	EnemyAgent(btDynamicsWorld* dynWorld);
	~EnemyAgent();
	void SetState(const type_info& stateType);
	AbstractEnemyState* GetState() const;
	void update(float pStep) override;
	MgeRigidbody* GetTarget() const;
	MgeRigidbody* GetOwner() const;
	void SetTarget(GameObject* target);
	void SetTarget(MgeRigidbody* target);
	void setOwner(GameObject* pGameObject) override;
	bool GetSeesTarget() const;
	btDynamicsWorld* GetDynWorld() const;
	void SetNavigationPath(NavigationPath* path);
	glm::vec3 GetStartPos() const;
	glm::quat GetStartRot() const;
	void SetPath(NavigationPath* path);
	NavigationNode* GetClosestNode() const;
	NavigationNode * GetTargetNode() const;
	NavigationNode * GetCurrentNode() const;
	void SetTargetNode(NavigationNode* node);
	void SetCurrentNode(NavigationNode* node);
private:
	bool SeesTarget() const;
	std::map <std::type_index, AbstractEnemyState *>	_stateCache;
	AbstractEnemyState *								_currentState;
	bool												_seesTarget;
	int													_sightConeAngle			= 90;
	int													_sightRange				= 10;
	float												_slerpSpeed				= 1.0f;
	glm::vec3											_startPos;
	glm::quat											_startRot;
	RigidBody*											_ownerRigidBody;
	MgeRigidbody*										_targetRB;
	MgeRigidbody*										_ownerRB;
	btDynamicsWorld*									_dynWorld;
	NavigationPath*										_path;
	NavigationNode*										_currentNode;
	NavigationNode*										_targetNode;
};
