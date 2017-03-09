#pragma once
#include <BulletDynamics\Dynamics\btRigidBody.h>
#include <mge/core/GameObject.hpp>

class MgeRigidbody : public btRigidBody
{
private:
	GameObject* _gameObject;

public:
	MgeRigidbody(const btRigidBodyConstructionInfo& constructionInfo, GameObject* gameObject);
	GameObject* getGameObject() const;
	btRigidBody* getMovableRigidBody() const;
};

