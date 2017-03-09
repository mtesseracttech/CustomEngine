#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "LinearMath/btDefaultMotionState.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

class RigidBody;
class AbstractBehaviour;
class AbstractMaterial;
class World;
class Mesh;

/**
* A GameObject wraps all data required to display an (interactive / dynamic) object, but knows nothing about OpenGL or rendering.
* You will need to alter this class to add colliders etc.
*/
class GameObject
{
public:
	GameObject(std::string pName = NULL, glm::vec3 pPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~GameObject();

	void setName(std::string pName);
	std::string getName() const;

	//contains local rotation, scale, position
	const glm::mat4& getTransform()			const;
	void setTransform(const glm::mat4& pTransform);

	//access just the local position
	glm::vec3 getLocalPosition()			const;
	glm::quat getLocalRotation()			const;
	glm::quat getLocalRotationSlow()		const;

	void setLocalPosition(const glm::vec3 pPosition);
	void setLocalRotation(const glm::quat rotation);

	glm::vec3 getTransformRight()			const;
	glm::vec3 getTransformUp()				const;
	glm::vec3 getTransformForward()			const;

	void setTransformForward(glm::vec3 fwd);

	//get the objects world position by combining transforms
	//expensive operations!! Improvement would be to cache these transforms!
	glm::vec3 getWorldPosition()			const;
	glm::quat getWorldRotation()			const;
	glm::mat4 getWorldTransform()			const;

	glm::vec3 getWorldTransformRight()		const;
	glm::vec3 getWorldTransformUp()			const;
	glm::vec3 getWorldTransformForward()	const;

	//change local position, rotation, scaling
	void translate(glm::vec3 pTranslation);
	void rotate(float pAngle, glm::vec3 pAxis);
	//DO NOT USE IN FINAL GAME, ROTATION FUNCTIONS MAKE GAMEOBJECT LOSE SCALE!
	void scale(glm::vec3 pScale);

	//mesh and material should be shared as much as possible
	void setMesh(Mesh* pMesh);
	Mesh* getMesh()									const;

	void setMaterial(AbstractMaterial* pMaterial);
	AbstractMaterial* getMaterial()					const;

	btDefaultMotionState* getDefaultMotionState()	const;
	void setRigidBody(btCollisionShape* shape, float mass, btDynamicsWorld* world);
	void setMeshRigidBody(btDynamicsWorld* world);
	RigidBody* getRigidBody()						const;
	void removeRigidBody()							const;
	virtual void update(float pStep);

	void AddBehaviour(AbstractBehaviour* behaviour);
	void RemoveBehaviour(AbstractBehaviour* behaviour);
	void ClearBehaviours();
	void DeleteBehaviours();

	//child management
	//shortcut to set the parent of pChild to ourselves
	void add(GameObject* pChild);
	//shortcut to set the parent of pChild to NULL
	void remove(GameObject* pChild);

	virtual void setParent(GameObject* pGameObject);
	GameObject* getParent();

	int getChildCount();
	GameObject* getChildAt(int pIndex);
	void enableDebugging();
	void printDebug();
	void deleteObject(const GameObject* pObject);


	//light
	void AdjustPosition();
	virtual void OnCollision(const btCollisionObject* other);

protected:
	std::string							_name;
	glm::mat4							_transform;
	GameObject*							_parent;
	std::vector<GameObject*>			_children;
	Mesh*								_mesh;
	AbstractMaterial*					_material;
	RigidBody*							_rigidBody;
	btDefaultMotionState*				_defaultMotionState;
	std::vector<AbstractBehaviour*>		_behaviours;
	//update children list administration
	void _innerAdd(GameObject* pChild);
	void _innerRemove(GameObject* pChild);
private:
	GameObject(const GameObject&);
	GameObject& operator=(const GameObject&);
	bool						_debug;
	glm::mat4 floatToMat4(float* Pmatrix);
};

#endif // GAMEOBJECT_H
