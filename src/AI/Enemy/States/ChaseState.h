#pragma once
#include "AbstractEnemyState.h"
class ChaseState :
	public AbstractEnemyState
{
public:
	ChaseState(EnemyAgent* agent);
	~ChaseState();
	void Start()	override;
	void Stop()		override;
	void Update(float deltaTime)	override;
};

