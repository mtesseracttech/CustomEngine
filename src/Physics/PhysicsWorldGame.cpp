#include "PhysicsWorldGame.h"
#include "mge/core/Renderer.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "Physics/RigidBody.h"

#include "mge/core/Camera.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"

#include "btBulletDynamicsCommon.h"
#include "Physics/MgeRigidbody.h"
#include "mge/behaviours/OrbitBehavior.h"
#include "AI/Enemy/EnemyAgent.h"
#include "glm.hpp"
#include "mge/behaviours/OrbCamera.h"
#include "mge/behaviours/ShootingBehaviourTeapot.h"
#include "mge/materials/LitMaterial.h"


PhysicsWorldGame::PhysicsWorldGame(): _hud(nullptr), _physicsWorld(nullptr)
{
}

void PhysicsWorldGame::initialize()
{
	//Setting up the core part
	AbstractGame::initialize();

	//Setting up HUD
	cout << "Initializing HUD" << endl;
	DebugHud::SetupInstance(_window);
	_hud = DebugHud::GetInstance();
	cout << "HUD initialized." << endl << endl;
}

void PhysicsWorldGame::_initializeWorld()
{
	cout << "Initializing Physics World!" << endl;
	_world = new PhysicsWorld();
	_physicsWorld = static_cast<PhysicsWorld*>(_world);
	cout << "Done Initializing Physics World!" << endl;
}

//build the game _world
void PhysicsWorldGame::_initializeScene()
{
	_renderer->setClearColor(0, 0, 0);

	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(0, 6, 7));
	camera->rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0));
	_physicsWorld->add(camera);
	_physicsWorld->setMainCamera(camera);

	//MESHES

	//load a bunch of meshes we will be using throughout this demo
	//each mesh only has to be loaded once, but can be used multiple times:
	//F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
	//Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
	Mesh* planeMeshDefault	= Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	Mesh* suzannaMeshF		= Mesh::load(config::MGE_MODEL_PATH + "suzanna_flat.obj");
	Mesh* teapotMeshS		= Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");

	//MATERIALS

	AbstractMaterial* colorMaterial		= new ColorMaterial(glm::vec3(0.2f, 0, 0.2f));
	AbstractMaterial* textureMaterial	= new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "land.jpg"));
	AbstractMaterial* textureMaterial2	= new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));

	// Can't use this on my macbook air, driver shits itself
	//AbstractMaterial* litMaterial = new LitMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"),
	//	Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"), false, Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"), false);
	//PointLight * pointLight = new PointLight("meh", glm::vec3(0, 5, 0), glm::vec3(1, 1, 1));

	//SCENE SETUP
	/**/
	unsigned int size = 20;
	for (size_t x = 0; x < size; x++)
	{
		for (size_t z = 0; z < size; z++)
		{
			glm::vec3 pos(2*(static_cast<int>(x) - static_cast<int>(size / 2)), 0, 2*(static_cast<int>(z) - static_cast<int>(size / 2)));
			GameObject* plane = new GameObject("plane" + (x + z), pos);
			plane->setMesh(planeMeshDefault);
			plane->setMaterial(textureMaterial);
			plane->setMeshRigidBody(_physicsWorld->GetDynamicsWorld()); //ONLY USE THE MESHRIGIDBODY FOR STATIC OBJECTS!
			_physicsWorld->add(plane);
			cout << "Position: " << pos.x << ", " << pos.z << endl;
		}
	}

	GameObject* teapot = new GameObject("teapot", glm::vec3(-3, 1, 0));
	teapot->setMesh(teapotMeshS);
	teapot->setMaterial(textureMaterial2);
	teapot->setRigidBody(new btBoxShape(btVector3(0.5, 0.5, 0.5)), 1, _physicsWorld->GetDynamicsWorld());
	if (btRigidBody* rb = teapot->getRigidBody()->GetRB())
	{
		rb->setDamping(0.5, 1);
		rb->setAngularFactor(btVector3(0, 0, 0));
	}
	teapot->AddBehaviour(new ShootingBehaviourTeapot(_physicsWorld));
	teapot->AddBehaviour(new KeysBehaviour());
	_physicsWorld->add(teapot);

	GameObject* monkey = new GameObject("monkey", glm::vec3(0, 1, 0));
	monkey->setMesh(suzannaMeshF);
	monkey->setMaterial(colorMaterial);
	monkey->setRigidBody(new btBoxShape(btVector3(1, 1, 1)), 1, _physicsWorld->GetDynamicsWorld());
	{
		EnemyAgent* agent1 = new EnemyAgent(_physicsWorld->GetDynamicsWorld());
		agent1->SetTarget(teapot);
		monkey->AddBehaviour(agent1);
		NavigationPath * path1 = new NavigationPath();
		NavigationNode * node0 = new NavigationNode(glm::vec3( 10, 1,  10));
		NavigationNode * node1 = new NavigationNode(glm::vec3(-10, 1,  10));
		NavigationNode * node2 = new NavigationNode(glm::vec3(-10, 1, -10));
		NavigationNode * node3 = new NavigationNode(glm::vec3( 10, 1, -10));
		node0->addNeighbours(node1)->addNeighbours(node2)->addNeighbours(node3);
		node1->addNeighbours(node2)->addNeighbours(node3)->addNeighbours(node0);
		node2->addNeighbours(node3)->addNeighbours(node0)->addNeighbours(node2);
		node3->addNeighbours(node0)->addNeighbours(node1)->addNeighbours(node2);
		path1->AddNode(node0)->AddNode(node1)->AddNode(node2)->AddNode(node3);
		agent1->SetPath(path1);
		HighlightNodes(path1);
	}

	_physicsWorld->add(monkey);

	camera->AddBehaviour(new OrbCamera(10, -88, 0, 2, teapot, _window));
}

void PhysicsWorldGame::_render()
{
	AbstractGame::_render();
	_updateHud();
}

void PhysicsWorldGame::_update(float deltaTime)
{
	_physicsWorld->SimulateWorld(deltaTime);
	//cout << "Simulated the Dynamicsworld for " << pStep << " second" << endl;
	AbstractGame::_update(deltaTime);
}

PhysicsWorldGame::~PhysicsWorldGame()
{
}

void PhysicsWorldGame::_updateHud()
{
	string debugInfo = "";
	debugInfo += string("FPS:") + std::to_string(static_cast<int>(_fps)) + "\n";

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

void PhysicsWorldGame::HighlightNodes(NavigationPath* path1)
{
	cout << "Spawning navnode highlights!" << std::endl;
	Mesh * GizmoMesh = Mesh::load(config::MGE_MODEL_PATH + "Axis_Sphere.obj");
	ColorMaterial * GizmoMaterial = new ColorMaterial(glm::vec3(0.0, 0.0, 1.0f));

	for(int i = 0; i < path1->GetNodeCount(); i++)
	{
		GameObject* gizmo = new GameObject("gizmo", path1->GetNodeAt(i)->getPosition());
		gizmo->scale(glm::vec3(0.2, 0.2, 0.2));
		gizmo->setMesh(GizmoMesh);
		gizmo->setMaterial(GizmoMaterial);
		_physicsWorld->add(gizmo);
	}
}
