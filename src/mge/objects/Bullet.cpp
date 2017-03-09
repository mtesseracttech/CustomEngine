#include "Bullet.h"
#include "Physics/MgeRigidbody.h"


Bullet::Bullet(glm::vec3 pForward,std::string pName, glm::vec3 pPosition, float pSpeed) : GameObject(pName, pPosition), _speed(pSpeed), _mesh(NULL), _material(NULL), _timer(1),_forwardVector(pForward)
{
}

Bullet::~Bullet()
{}

void Bullet::OnCollision(const btCollisionObject* other)
{
	/**/
	if (const MgeRigidbody* rb = dynamic_cast<const MgeRigidbody *>(other))
	{
		if(rb->getGameObject()->getName() == "enemy")
		{
			
			rb->getGameObject()->removeRigidBody();
			rb->getGameObject()->remove(rb->getGameObject());
			rb->getGameObject()->deleteObject(rb->getGameObject());

			RemoveBullet();
		}
		
		else
		{
			std::cout << _name << " collided with " << rb->getGameObject()->getName() << std::endl;
			RemoveBullet();
		}
		
	}
	/**/
}

void Bullet::RemoveBullet()
{
	//removing bullet
	removeRigidBody();
	remove(this);
	deleteObject(this);
}


void Bullet::update(float pStep)
{
	translate(_forwardVector  * _speed  );
	if(getRigidBody() != nullptr)
	{
		btRigidBody* rb = this->getRigidBody()->GetRB();
		//DONE: Bullet moves and kills enemy
		rb->setLinearVelocity(btVector3(_forwardVector.x, _forwardVector.y, _forwardVector.z)* _speed * 100);
		
		_timer -= 0.01;
		if (_timer <= 0)//or it collided with object
		{
			RemoveBullet();
		}
	}
	
	
}


