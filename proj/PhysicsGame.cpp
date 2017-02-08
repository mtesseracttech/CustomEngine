#include "PhysicsGame.h"
#include <iostream>
#include <glm/glm.hpp>
#include "mge/config.hpp"
#include "mge/util/DebugHud.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

using namespace std;

PhysicsGame::PhysicsGame() : AbstractGame(), _hud(0)
{
}


void PhysicsGame::initialize() 
{
	AbstractGame::initialize();
	initializeBullet();
	cout << "Initializing HUD" << endl;
	_hud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;
}

void PhysicsGame::initializeBullet()
{
	cout << "Initializing Bullet Physics Engine" << endl;
	cout << "1 = Initialized, 0 = Not Initialized" << endl;
	_broadphase				= new btDbvtBroadphase();
	cout << "Broadphase Initialized Status: "	<< (_broadphase				!= nullptr) << endl;
	_collisionConfiguration = new btDefaultCollisionConfiguration();
	cout << "Collision Configuration Status: "	<< (_collisionConfiguration != nullptr) << endl;
	_dispatcher				= new btCollisionDispatcher(_collisionConfiguration);
	cout << "Collision Dispatcher Status: "		<< (_dispatcher				!= nullptr) << endl;
	_solver					= new btSequentialImpulseConstraintSolver;
	cout << "Constraint Solver Status: "		<< (_solver					!= nullptr) << endl;
	_dynamicsWorld			= new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
	cout << "Dynamics World Status: "			<< (_dynamicsWorld			!= nullptr) << endl;
	_dynamicsWorld->setGravity(btVector3(0, -10, 0));
	btVector3 grav = _dynamicsWorld->getGravity();
	cout << "Dynamics World Gravity Set to: (" << grav.getX() << "," << grav.getY() << "," << grav.getZ() << ")" << endl;
}

void PhysicsGame::_initializeScene()
{
	_renderer->setClearColor(0, 0, 0);

	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(0, 6, 7));
	camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);

	//MESHES

	//load a bunch of meshes we will be using throughout this demo
	//each mesh only has to be loaded once, but can be used multiple times:
	//F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
	Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	Mesh* suzannaMeshF = Mesh::load(config::MGE_MODEL_PATH + "suzanna_flat.obj");
	Mesh* teapotMeshS = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");

	//MATERIALS

	AbstractMaterial* colorMaterial = new ColorMaterial(glm::vec3(0.2f, 0, 0.2f));
	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "land.jpg"));
	AbstractMaterial* textureMaterial2 = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));

	//SCENE SETUP

	GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
	plane->scale(glm::vec3(5, 5, 5));
	plane->setMesh(planeMeshDefault);
	plane->setMaterial(textureMaterial);
	_world->add(plane);

	GameObject* teapot = new GameObject("teapot", glm::vec3(-3, 1, 0));
	teapot->setMesh(teapotMeshS);
	teapot->setMaterial(textureMaterial2);
	teapot->setBehaviour(new KeysBehaviour());
	_world->add(teapot);

	GameObject* monkey = new GameObject("monkey", glm::vec3(3, 1, 0));
	monkey->setMesh(suzannaMeshF);
	monkey->setMaterial(colorMaterial);
	monkey->setBehaviour(new RotatingBehaviour());
	_world->add(monkey);

	camera->setBehaviour(new LookAt(teapot));
}

void PhysicsGame::_render() 
{
	AbstractGame::_render();
	_updateHud();
}

void PhysicsGame::_updateHud() 
{
	string debugInfo = "";
	debugInfo += string("FPS:") + to_string((int)_fps) + "\n";
	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

PhysicsGame::~PhysicsGame()
{
	delete _dynamicsWorld;
	delete _solver;
	delete _collisionConfiguration;
	delete _dispatcher;
	delete _broadphase;
}

auto toBtVector = [&](const glm::vec3 &vec) -> btVector3
{
	btVector3 bt(vec.x, vec.y, vec.z);
	return bt;
};

auto toGLMVector = [&](const btVector3 &vec) -> glm::vec3
{
	glm::vec3 glmVec;
	glmVec.x = vec.getX();
	glmVec.y = vec.getY();
	glmVec.z = vec.getZ();
	return glmVec;
};
