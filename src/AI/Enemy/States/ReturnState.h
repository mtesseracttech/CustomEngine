#pragma once
#include "AbstractEnemyState.h"
#include "glm/glm.hpp"

class ReturnState :
	public AbstractEnemyState
{
public:
	ReturnState(EnemyAgent* agent);
	~ReturnState();
	void Start()	override;
	void Stop()		override;
	void Update(float deltaTime)	override;
	void GoToTarget(glm::vec3 curVec, glm::vec3 tarVec);
};

