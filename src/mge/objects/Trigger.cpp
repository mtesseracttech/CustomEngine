#include "Trigger.h"


Trigger::Trigger(std::string pName, glm::vec3 pPosition) : GameObject(pName, pPosition),_name(pName),_sound()
{}

Trigger::~Trigger()
{
	
}


void Trigger::OnCollision(const btCollisionObject* other)
{
	if (const MgeRigidbody* rb = dynamic_cast<const MgeRigidbody *>(other))
	{
		if (rb->getGameObject()->getName() == "interactable"&& _name=="triggerInter")
		{
			//TODO:something with trigger and interactable
			_sound.Play(SoundEffect::Trigger_Activated, 100);
		}
		if(rb->getGameObject()->getName() == "player"&& _name == "triggerFeed1")
		{
			//todo:show wasd screen
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				removeRigidBody();
				remove(this);
				deleteObject(this);
			}
		}
		if (rb->getGameObject()->getName() == "player"&& _name == "triggerFeed2")
		{
			//todo:show shooting screen
		}
		if (rb->getGameObject()->getName() == "player"&& _name == "triggerWin")
		{
			//Bart hit me :..( So did you....
			//todo::transition to new level
		}
	}
}

void Trigger::update(float pStep)
{

}


