#ifndef ORBCAMERA_H
#define ORBCAMERA_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <glm/glm.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


class OrbCamera : public AbstractBehaviour
{
public:
	OrbCamera(float pDistance, float pMinTangle, float pMaxTangle, float pSpeed, GameObject *pTarget, sf::RenderWindow* pWindow);
	virtual ~OrbCamera();
	virtual void update(float pStep);
private:
	void UpdateMousePosition();

	GameObject *_target;
	sf::RenderWindow* _window;
	glm::vec2 _mousePos;
	glm::vec3 _posiition;
	glm::vec3 _targetPosition;
	glm::vec2 rot;

	float _distance;
	float _minTangle;
	float _maxTangle;
	float _speed;
};

#endif ORBCAMERA_H
