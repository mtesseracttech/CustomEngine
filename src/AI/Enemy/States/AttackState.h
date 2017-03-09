#pragma once
#include "AbstractEnemyState.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
class btDynamicsWorld;

class AttackState :
	public AbstractEnemyState
{
public:
	AttackState(EnemyAgent* agent, btDynamicsWorld* dynWorld);
	~AttackState();
	void Start()	override;
	void Stop()		override;
	void RotateToTarget();
	void Shoot(const btCollisionWorld::ClosestRayResultCallback& callback, float deltaTime);
	void LineOfSightHandler(float deltaTime);
	void Update(float deltaTime)	override;
private:
	btDynamicsWorld* _dynWorld;
	float const			_slerpSpeed = 0.1f;
};

