#include <glm.hpp>
#include "mge/behaviours/LookAt.hpp"
#include "mge/core/GameObject.hpp"

LookAt::LookAt(GameObject* pTarget, float pDistance): AbstractBehaviour(), _target(pTarget),_distance(pDistance)
{
}

LookAt::~LookAt()
{}


void LookAt::update(float pStep)
{
 	_targetPosition = _target->getLocalPosition();
	UpdatePosition();
    
}

void LookAt::UpdatePosition()
{

		glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(_targetPosition.y), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(), glm::radians(_targetPosition.x), glm::vec3(1, 0, 0));

		glm::vec3 transform = glm::vec3(0, 0, _distance);
		glm::vec3 newPos = glm::vec3( glm::vec4(transform, 1.0f));
		_owner->setLocalPosition(newPos + _targetPosition);

		//the other way then you would expect, the camera is flipped over the z axis we are staring out of its a.. basically ;)
		//this is due to how the perspective matrix is set up
		glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - _target->getLocalPosition());
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
		glm::vec3 up = glm::normalize(glm::cross(forward, right));

		_owner->setTransform(
			glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1)));
}

