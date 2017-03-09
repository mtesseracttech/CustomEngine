#pragma once
class EnemyAgent;

class AbstractEnemyState
{
public:
	AbstractEnemyState(EnemyAgent* agent);
	virtual ~AbstractEnemyState();
	virtual void Start()  = 0;
	virtual void Stop()   = 0;
	virtual void Update(float deltaTime) = 0;
protected:
	EnemyAgent*				_agent;
};

