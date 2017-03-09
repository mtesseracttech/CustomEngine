#include "MgeRigidbody.h"
#include "RigidBody.h"



MgeRigidbody::MgeRigidbody(const btRigidBodyConstructionInfo& constructionInfo, GameObject* gameObject) : btRigidBody(constructionInfo), _gameObject(gameObject)
{
}

GameObject* MgeRigidbody::getGameObject() const
{
	return _gameObject;
}

btRigidBody* MgeRigidbody::getMovableRigidBody() const
{
	return _gameObject->getRigidBody()->GetRB();
}


