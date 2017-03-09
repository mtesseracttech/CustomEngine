#ifndef TRIGGER_H
#define TRIGGER_H

#include "mge/core/GameObject.hpp"
#include "Physics/RigidBody.h"
#include "mge/audio/SoundPlayer.hpp"
#include "mge/util/DebugHud.hpp"

class Trigger : public GameObject
{
public:
	Trigger(std::string pName = nullptr, glm::vec3 pPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~Trigger();
	void update(float pStep) override;
	void OnCollision(const btCollisionObject* other) override;


private:
	std::string _name;
	SoundPlayer _sound;
};


#endif TRIGGER_H
