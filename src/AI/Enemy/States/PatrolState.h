#pragma once
#include "AbstractEnemyState.h"
#include "AI/Navigation/NavigationPath.h"

class PatrolState :
	public AbstractEnemyState
{
public:
	PatrolState(EnemyAgent* agent);
	~PatrolState();
	void Start()	override;
	void Stop()		override;
	void FollowPath();
	void Update (float deltaTime)	override;
	NavigationNode* GetClosestNode() const;
	void RotateAroundAxis() const;
	void SetPath(NavigationPath* path);
private:
	NavigationPath *			_path;
	bool						_pathMode;
	bool						_alignedAtStart;
	float						_speed;
	float				const	MaxSpeed			= 0.2f;
	float				const	SlerpSpeed			= 0.05f;
	float				const	IdleRotationSpeed	= 0.05f;
};

