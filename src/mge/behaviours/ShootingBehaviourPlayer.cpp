#include "ShootingBehaviourPlayer.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Time.hpp>
#include "mge/core/GameObject.hpp"
#include "BulletCollision/CollisionShapes/btBoxShape.h"

ShootingbehaviourPlayer::ShootingbehaviourPlayer(float pSpeed, Mesh* pBulletMesh, AbstractMaterial* pBulletMaterial,btDynamicsWorld* pDynamicWorld,sf::RenderWindow* pWindow) : AbstractBehaviour(),
_bulletSpeed(pSpeed), _shootingDirection(), _shootCounter(0),_bulletObject(),_bulletMesh(pBulletMesh),_bulletMaterial(pBulletMaterial),_dynamicWorld(pDynamicWorld),_window(pWindow),_sound()
{
}

ShootingbehaviourPlayer::~ShootingbehaviourPlayer()
{}

void ShootingbehaviourPlayer::update(float pStep)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button(0)))
	{
		if (_currentAmo > 0)
		{
			//losing doesn't define us.
			//work hard and do better tomorrow
			_shootCounter -= sf::Time().asSeconds() *pStep;
			if (_shootCounter <= 0)
			{
				_sound.Play(SoundEffect::Player_Shoot, 100);
				_shootingDirection = _owner->getTransform()[2];
				_shootCounter = _timeBetweenShooting;
				glm::vec3 forward = _owner->getWorldPosition() + glm::normalize(_shootingDirection) *1.9f;
				//creating a bullet
				_bulletObject = new Bullet(_shootingDirection,"bullet", forward ,_bulletSpeed );
				//giving a mesh
				_bulletObject->setMesh(_bulletMesh);
				//giving material
				_bulletObject->setMaterial(_bulletMaterial);
				//giving a rigidbody
				_bulletObject->setRigidBody(new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), 1, _dynamicWorld);
				//adding to the world
				_owner->getParent()->add(_bulletObject);
				//decrease ammo
				_currentAmo -= 1;
				std::cout << _currentAmo << std::endl;
			}
		}
	}
	else
	{
		//reset counter
		_shootCounter = 0;
	}
}



