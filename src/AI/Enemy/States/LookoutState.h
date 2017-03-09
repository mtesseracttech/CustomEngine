#pragma once
#include "AbstractEnemyState.h"
class LookoutState : public AbstractEnemyState
{
public:
	LookoutState(EnemyAgent* agent);
	~LookoutState();
	void Start()	override;
	void Stop()		override;
	void Update(float deltaTime)	override;
};

