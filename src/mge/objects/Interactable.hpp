#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "mge/core/GameObject.hpp"
#include "Physics/MgeRigidbody.h"
#include "Physics/RigidBody.h"
class Interactable : public GameObject
{
public:
	Interactable(GameObject* interactObject,std::string pName = nullptr, glm::vec3 pPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~Interactable();
	void update(float pStep) override;
	void OnCollision(const btCollisionObject* other) override;

private:
	std::string _name;
	bool _activated = false;
	GameObject* _object;
};


#endif INTERACTABLE_H
