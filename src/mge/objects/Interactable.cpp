#include "Interactable.hpp"


Interactable::Interactable(GameObject* pObject,std::string pName, glm::vec3 pPosition) : GameObject(pName, pPosition), _name(pName),_object(pObject)
{

}

Interactable::~Interactable()
{

}

void Interactable::OnCollision(const btCollisionObject* other)
{
	if (const MgeRigidbody* rb = dynamic_cast<const MgeRigidbody *>(other))
	{
		if (rb->getGameObject()->getName() == "triggerInter")
		{
			//TODO:somethind with trigger
			_object->removeRigidBody();
			remove(_object);
			deleteObject(_object);
			_activated = true;
		}
	}
}

void Interactable::update(float pStep)
{
	if(getRigidBody()->GetRB()->isActive())
	{
		AdjustPosition();
	}
	//todo: optimize when interacted disable rigidbody
	if (_activated && getRigidBody() != nullptr)
	{
		removeRigidBody();
		remove(this);
		deleteObject(this);
	}
}


