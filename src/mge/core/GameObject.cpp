//#include <cassert>
#include <iostream>
#include <set>
#include <glm/gtx/matrix_decompose.hpp>
using namespace std;

#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <Physics/RigidBody.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include <algorithm>

GameObject::GameObject(std::string pName, glm::vec3 pPosition )
	: _name(pName), _transform(glm::translate(pPosition)),
	  _parent(NULL), _children(), _mesh(NULL), _material(NULL), _rigidBody(NULL), _defaultMotionState(NULL), _debug(false), _behaviours()
{
	btTransform trans;
	trans.setFromOpenGLMatrix(glm::value_ptr(_transform));
	_defaultMotionState = new btDefaultMotionState(trans);
	_behaviours = std::vector<AbstractBehaviour*>();
}

GameObject::~GameObject()
{
    //detach all children

    cout << "GC running on:" << _name << endl;

    while (_children.size() > 0) {
        GameObject* child = _children[0];
        remove (child);
        delete child;
    }
    //do not forget to delete behaviour, material, mesh, collider manually if required!
}

void GameObject::setName (std::string pName)
{
    _name = pName;
}

std::string GameObject::getName() const
{
	return _name;
}

void GameObject::setTransform (const glm::mat4& pTransform)
{
    _transform = pTransform;
}

const glm::mat4& GameObject::getTransform() const
{
    return _transform;
}

glm::vec3 GameObject::getLocalPosition() const
{
	return glm::vec3(_transform[3]);
}

void GameObject::setLocalPosition (const glm::vec3 pPosition)
{
    _transform[3] = glm::vec4 (pPosition,1);
}

glm::quat GameObject::getLocalRotation() const
{
	return glm::quat_cast(_transform);
}

void GameObject::setLocalRotation(const glm::quat rotation)
{
	glm::vec3 pos = _transform[3];
	glm::mat4 newMat = glm::mat4_cast(rotation);
	newMat[3] = glm::vec4(pos, 1);
	_transform = newMat;
}


//PROBABLY REALLY FUCKING SLOW, USE WITH CAUTION!
glm::quat GameObject::getLocalRotationSlow() const
{
	glm::mat4 transformation = _transform;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transformation, scale, rotation, translation, skew, perspective);
	return rotation;
}

glm::vec3 GameObject::getTransformRight() const
{
	return glm::vec3(_transform[0]);
}

glm::vec3 GameObject::getTransformUp() const
{
	return glm::vec3(_transform[1]);
}

glm::vec3 GameObject::getTransformForward() const
{
	return glm::vec3(_transform[2]);
}

void GameObject::setTransformForward(glm::vec3 fwd)
{
	glm::vec3 forward = glm::normalize(fwd);
	glm::vec3 right = glm::normalize(glm::cross(getWorldTransformUp(), fwd));
	glm::vec3 up = glm::normalize(glm::cross(fwd, right));

	setTransform(glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(getLocalPosition(), 1)));
}

void GameObject::setMaterial(AbstractMaterial* pMaterial)
{
    _material = pMaterial;
}

AbstractMaterial * GameObject::getMaterial() const
{
    return _material;
}

void GameObject::setMesh(Mesh* pMesh)
{
	_mesh = pMesh;
}

Mesh * GameObject::getMesh() const
{
    return _mesh;
}

btDefaultMotionState* GameObject::getDefaultMotionState() const
{
	return _defaultMotionState;
}

void GameObject::setRigidBody(btCollisionShape* shape, float mass, btDynamicsWorld* world)
{
	if(_rigidBody == nullptr)
	{
		if(mass == 0)
		{
			_rigidBody = new RigidBody(this, shape, getDefaultMotionState(), world);
		}
		else
		{
			_rigidBody = new RigidBody(this, shape, getDefaultMotionState(), mass, world);
		}
	}
}

//WARNING THIS SHIT IS FUCKING STATIC
void GameObject::setMeshRigidBody(btDynamicsWorld* world)
{
	if(_rigidBody == nullptr)
	{
		btCollisionShape * btShape = new btBvhTriangleMeshShape(_mesh->GetCollisionMesh(), true);
		btShape->setMargin(0.05f);
		_rigidBody = new RigidBody(this, btShape, getDefaultMotionState(), world);
	}
}

RigidBody* GameObject::getRigidBody() const
{
	return _rigidBody;
}

void GameObject::removeRigidBody() const
{
	if (_rigidBody) delete _rigidBody;
}

void GameObject::setParent (GameObject* pParent) {
    //remove from previous parent
    if (_parent != NULL) {
        _parent->_innerRemove(this);
        _parent = NULL;
    }

    //set new parent
    if (pParent != NULL) {
        _parent = pParent;
        _parent->_innerAdd(this);
    }
}

GameObject* GameObject::getParent() {
    return _parent;
}

void GameObject::_innerAdd(GameObject* pChild)
{
	//set new parent
	pChild->_parent = this;
	_children.push_back(pChild);
}

void GameObject::_innerRemove (GameObject* pChild) {
    for (auto i = _children.begin(); i != _children.end(); ++i) {
        if (*i == pChild) {
            (*i)->_parent = NULL;
            _children.erase(i);
            return;
        }
    }
}

void GameObject::add (GameObject* pChild) {
    pChild->setParent(this);
}

void GameObject::remove (GameObject* pChild) {
    pChild->setParent(NULL);
}

////////////

//costly operation, use with care
glm::vec3 GameObject::getWorldPosition() const
{
	return glm::vec3(getWorldTransform()[3]);
}

//costly operation, use with care
glm::quat GameObject::getWorldRotation() const
{
	return glm::quat_cast(getWorldTransform());
}

//costly operation, use with care
glm::mat4 GameObject::getWorldTransform() const
{
    if (_parent == NULL) return _transform;
    else return _parent->getWorldTransform() * _transform;
}

glm::vec3 GameObject::getWorldTransformRight() const
{
	return glm::vec3(getWorldTransform()[0]);
}

glm::vec3 GameObject::getWorldTransformUp() const
{
	return glm::vec3(getWorldTransform()[1]);
}

glm::vec3 GameObject::getWorldTransformForward() const
{
	return glm::vec3(getWorldTransform()[2]);
}

////////////

void GameObject::translate(glm::vec3 pTranslation)
{
	setTransform(glm::translate(_transform, pTranslation));
}

void GameObject::scale(glm::vec3 pScale)
{
	setTransform(glm::scale(_transform, pScale));
}

void GameObject::rotate(float pAngle, glm::vec3 pAxis)
{
	setTransform(glm::rotate(_transform, pAngle, pAxis));
}

//all game objects are updated in a backward loop, first the behaviour is updated, then all children are updated
void GameObject::update(float pStep)
{
	bool hasRb = _rigidBody != nullptr;
	if (hasRb && _rigidBody->GetRB()->isActive())
	{
		//AdjustPosition();
		/**/
		//Create empty transform
		btTransform rbTrans;
		//Set fill it with data from the RB
		_rigidBody->GetMotionState()->getWorldTransform(rbTrans);
		//Write the data to the GO's transform
		rbTrans.getOpenGLMatrix(value_ptr(_transform));
		/**/
	}

	if(_behaviours.size() > 0)
	{
		for (int i = _behaviours.size() - 1; i >= 0; --i)
		{
			_behaviours[i]->update(pStep);
		}
	}

	for (int i = _children.size()-1; i >= 0; --i)
	{
		_children[i]->update(pStep);
	}

	if (hasRb && _rigidBody->GetRB()->isActive())
	{
		//AdjustPosition();
		/**/
		//Create empty transform
		btTransform rbTrans;
		//Fill it with the data from the GO's Transform
		rbTrans.setFromOpenGLMatrix(value_ptr(_transform));
		//Fill the RB's Center of Mass Transform with the info
		_rigidBody->GetRB()->setCenterOfMassTransform(rbTrans);
		/**/
	}

	if (_debug) printDebug();
}

void GameObject::deleteObject(const GameObject* pObject)
{
	delete pObject;
}

glm::mat4 GameObject::floatToMat4(float* pMatrix)
{
	return glm::mat4(pMatrix[0], pMatrix[1], pMatrix[2], pMatrix[3],
		pMatrix[4], pMatrix[5], pMatrix[6], pMatrix[7],
		pMatrix[8], pMatrix[9], pMatrix[10], pMatrix[11],
		pMatrix[12], pMatrix[13], pMatrix[14], pMatrix[15]);
}

void GameObject::AdjustPosition()
{
	float mat[16];
	btTransform t;

	_rigidBody->GetMotionState()->getWorldTransform(t);

	t.getOpenGLMatrix(mat);
	setTransform(floatToMat4(mat));
}


void GameObject::AddBehaviour(AbstractBehaviour* behaviour)
{
	behaviour->setOwner(this);
	_behaviours.push_back(behaviour);
}

void GameObject::RemoveBehaviour(AbstractBehaviour* behaviour)
{
	_behaviours.erase(std::remove(_behaviours.begin(), _behaviours.end(), behaviour), _behaviours.end());
}

void GameObject::ClearBehaviours()
{
	_behaviours.clear();
}

void GameObject::DeleteBehaviours()
{
	if(_behaviours.size() > 0)
	{
		for (size_t i = _behaviours.size() - 1; i >= 0; --i)
		{
			delete _behaviours[i];
		}
	}
	ClearBehaviours();
}

int GameObject::getChildCount() {
    return _children.size();
}

GameObject* GameObject::getChildAt(int pIndex) {
    return _children[pIndex];
}

void GameObject::enableDebugging()
{
	_debug = true;
}

void GameObject::printDebug()
{

}

void GameObject::OnCollision(const btCollisionObject* other)
{
}



