#ifndef SHOOTINGBEHAVIOR_H
#define SHOOTINGBEHAVIOR_H

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"
#include "mge/objects/Bullet.h"
#include "mge/audio/SoundPlayer.hpp"

class ShootingbehaviourPlayer: public AbstractBehaviour
{
public:
	ShootingbehaviourPlayer(float pSpeed, Mesh* pBulletMesh, AbstractMaterial* pBulletMaterial, btDynamicsWorld* pDynamicWorld, sf::RenderWindow* pWindow);
	virtual ~ShootingbehaviourPlayer();
	virtual void update(float pStep);

private:
	sf::RenderWindow*			_window;
	float						_bulletSpeed;
	glm::vec3					_shootingDirection;
	float						_shootCounter;
	float						_timeBetweenShooting = 1 ;
	int							_currentAmo = 10;
	Bullet*						_bulletObject;
	Mesh*						_bulletMesh;
	AbstractMaterial*			_bulletMaterial;
	btDynamicsWorld*			_dynamicWorld;
	SoundPlayer					_sound;
};

#endif SHOOTINGBEHAVIOR_H