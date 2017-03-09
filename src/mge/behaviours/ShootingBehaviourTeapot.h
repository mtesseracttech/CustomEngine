#pragma once
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "Physics/PhysicsWorldGame.h"

class ShootingBehaviourTeapot : public AbstractBehaviour
{
public:
	ShootingBehaviourTeapot(PhysicsWorld* phyWorld);
	~ShootingBehaviourTeapot();
	void SpawnGizmo(glm::vec3 point);
	void update(float pStep) override;
	void setOwner(GameObject* pGameObject) override;
protected:
	btDynamicsWorld *	_dynWorld;
	PhysicsWorld *		_physicsWorld;
	Mesh*				_gizmoMesh;
	ColorMaterial*		_gizmoMaterial;
	vector<GameObject*>	_gizmoList;
};

