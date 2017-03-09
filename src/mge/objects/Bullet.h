#ifndef BULLET_H
#define BULLET_H

#include "mge/core/GameObject.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Physics/RigidBody.h"

class Bullet : public GameObject
{
public:
	Bullet(glm::vec3 pForwawrd ,std::string pName = NULL, glm::vec3 pPosition = glm::vec3(0.0f, 0.0f, 0.0f), float pSpeed = 20);
	virtual ~Bullet();
	virtual void update(float pStep);
	virtual void OnCollision(const btCollisionObject* other);

private:
	Mesh* _mesh;
	AbstractMaterial* _material;
	std::string _name;
	glm::vec3 _forwardVector;
	float _speed ;
	float _timer ;
	void RemoveBullet();
};

#endif //BULLET_H
#pragma once
