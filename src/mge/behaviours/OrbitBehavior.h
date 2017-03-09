#ifndef ORBITBEHAVIOR_H
#define ORBITBEHAVIOR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <glm/glm.hpp>


class OrbitBehavior : public AbstractBehaviour
{
public:
	OrbitBehavior(float pDistance, GameObject *pTarget);
	virtual ~OrbitBehavior();
	virtual void update(float pStep);
private:
	void UpdateMousePosition();

	GameObject *_target;
	glm::vec3 _posiition;
	glm::vec3 _targetPosition;
	glm::vec2 rot;

	float _distance;
};

#endif 
