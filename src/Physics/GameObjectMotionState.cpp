#include "GameObjectMotionState.h"

GameObjectMotionState::GameObjectMotionState(const btTransform &initialPosition, GameObject* gameObject)
{
	_gameObject = gameObject;
}

GameObjectMotionState::~GameObjectMotionState()
{
}

GameObject* GameObjectMotionState::getGameObject() const
{
	return _gameObject;
}

void GameObjectMotionState::getWorldTransform(btTransform& worldTrans) const
{
	_
}

void GameObjectMotionState::setWorldTransform(const btTransform& worldTrans)
{
}


